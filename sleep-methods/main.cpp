#include <x86intrin.h>
#include <fstream>
#include <chrono>
#include <time.h>
#include <functional>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <immintrin.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>

#define BILLION 1000000000L

const long int clock_ticks_per_seconds{sysconf(_SC_CLK_TCK)}; // Don't confuse with cpu ticks !

double getCpuMaxFrequency()
{
  unsigned int cpu_freq_khz;
  std::ifstream cpuinfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
  if (cpuinfo)
  {
    cpuinfo >> cpu_freq_khz;
    cpuinfo.close();
  }
  else
  {
    throw std::runtime_error("Unable to open cpuinfo_max_freq");
  }
  return cpu_freq_khz * 1.0e3; // convert from KHz to Hz
}

// const double cpu_frequency{getCpuMaxFrequency()}; // in Hz
const double cpu_frequency{2.7e9}; // according /proc/cpuinfo
double cpuTicksToTime(const unsigned long long &cpu_ticks)
{
  // Very not sure about that !
  return cpu_ticks / cpu_frequency;
}

uint64_t durationBetweenTimespecs(const timespec &start, const timespec &end)
{
  return BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
}

struct TimePoint
{
  TimePoint() : system_clock_time(std::chrono::system_clock::now()),
                high_resolution_clock_time(std::chrono::high_resolution_clock::now()),
                rtdsc_time{__rdtsc()}

  {
    clock_gettime(CLOCK_MONOTONIC, &clock_monotonic_time);
    clock_gettime(CLOCK_REALTIME, &clock_realtime_time);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock_process_cputime_id_time);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &clock_thread_cputime_id_time);
  }

  struct timespec clock_monotonic_time;
  struct timespec clock_realtime_time;
  struct timespec clock_process_cputime_id_time;
  struct timespec clock_thread_cputime_id_time;
  std::chrono::system_clock::time_point system_clock_time{};
  std::chrono::high_resolution_clock::time_point high_resolution_clock_time{};
  const unsigned long long rtdsc_time{};
};

struct Duration
{
  Duration(const TimePoint &start, const TimePoint &end)
      : clock_monotonic_duration(durationBetweenTimespecs(start.clock_monotonic_time, end.clock_monotonic_time)),
        clock_realtime_duration(durationBetweenTimespecs(start.clock_realtime_time, end.clock_realtime_time)),
        clock_process_cputime_id_duration(durationBetweenTimespecs(start.clock_process_cputime_id_time, end.clock_process_cputime_id_time)),
        clock_thread_cputime_id_duration(durationBetweenTimespecs(start.clock_thread_cputime_id_time, end.clock_thread_cputime_id_time)),
        system_clock_duration(std::chrono::duration_cast<std::chrono::nanoseconds>(end.system_clock_time - start.system_clock_time).count()),
        high_resolution_clock_duration(end.high_resolution_clock_time - start.high_resolution_clock_time),
        rtdsc_elapsed_ticks(end.rtdsc_time - start.rtdsc_time),
        rtdsc_elapsed_time{cpuTicksToTime(rtdsc_elapsed_ticks)}
  {
  }

  // See also https://linux.die.net/man/3/clock_gettime
  uint64_t clock_monotonic_duration;
  uint64_t clock_realtime_duration;
  uint64_t clock_process_cputime_id_duration;
  uint64_t clock_thread_cputime_id_duration;
  std::chrono::nanoseconds system_clock_duration{};
  std::chrono::high_resolution_clock::duration high_resolution_clock_duration{};
  const unsigned long long rtdsc_elapsed_ticks{};
  const double rtdsc_elapsed_time{};
};

struct BenchmarkCondition
{
  BenchmarkCondition(const std::string name, const double desired_duration, const std::function<void(const double)> &custom_sleep_function, const bool stress_system = false)
      : name_(name), desired_duration_(desired_duration), custom_sleep_function_(custom_sleep_function), stress_system_(stress_system)
  {
  }
  const std::string name_;
  const double desired_duration_;
  const std::function<void(const double)> custom_sleep_function_;
  const bool stress_system_;
};

void display_duration(const Duration &duration, const double desired_duration)
{
  printf("Desired %f -- Duration according: ", desired_duration);
  printf("CPU ticks %f; ", duration.rtdsc_elapsed_time);
  printf("high_resolution_clock::now %f; ", std::chrono::duration_cast<std::chrono::duration<double>>(duration.high_resolution_clock_duration).count());
  printf("system_clock %f; ", std::chrono::duration_cast<std::chrono::duration<double>>(duration.system_clock_duration).count());
  printf("CLOCK_MONOTONIC %f; ", static_cast<double>(duration.clock_monotonic_duration) / 1.0e9);
  printf("CLOCK_REALTIME %f; ", static_cast<double>(duration.clock_realtime_duration) / 1.0e9);
  printf("CLOCK_PROCESS_CPU_TIME_ID %f; ", static_cast<double>(duration.clock_process_cputime_id_duration) / 1.0e9);
  printf("CLOCK_THREAD_CPUTIME_ID %f; ", static_cast<double>(duration.clock_thread_cputime_id_duration) / 1.0e9);
  printf("Elapsed CPU tick = %llu;", duration.rtdsc_elapsed_ticks);

  printf("\n");
}

void with_rtdsc_sleep(const double seconds)
{
  bool finished{false};
  auto start = __rdtsc();
  while (!finished)
  {
    auto now = __rdtsc();
    if (cpuTicksToTime(now - start) >= seconds)
    {
      return;
    }
  }
}

void polling_clock(const double seconds, const clockid_t clock_id)
{
  timespec start_time_point{};
  clock_gettime(clock_id, &start_time_point);
  timespec end_time_point{};
  do
  {
    clock_gettime(clock_id, &end_time_point);
  } while (durationBetweenTimespecs(start_time_point, end_time_point) < seconds * BILLION);
}

void polling_clock_and_mm_pause(const double seconds, const clockid_t clock_id)
{
  timespec start_time_point{};
  clock_gettime(clock_id, &start_time_point);
  timespec end_time_point{};
  do
  {
    clock_gettime(clock_id, &end_time_point);
    _mm_pause();
  } while (durationBetweenTimespecs(start_time_point, end_time_point) < seconds * BILLION);
}

void with_clock_cpu_process_time_id(const double seconds)
{
  polling_clock(seconds, CLOCK_PROCESS_CPUTIME_ID);
}
void with_clock_realtime(const double seconds)
{
  polling_clock(seconds, CLOCK_REALTIME);
}

void with_clock_cpu_process_time_id_and_mm_pause(const double seconds)
{
  polling_clock_and_mm_pause(seconds, CLOCK_PROCESS_CPUTIME_ID);
}
void with_clock_realtime_and_mm_pause(const double seconds)
{
  polling_clock_and_mm_pause(seconds, CLOCK_REALTIME);
}

void with_sleep_for(const double seconds)
{
  std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}

void with_usleep(const double seconds)
{
  unsigned int microseconds = static_cast<unsigned int>(seconds * 1.0e6);
  usleep(microseconds);
}

void with_nanosleep(const double seconds)
{
  timespec ts;

  double intPart;
  double fracPart = modf(seconds, &intPart);
  // Fill the timespec structure
  ts.tv_sec = intPart;
  ts.tv_nsec = static_cast<long>(fracPart) * 1e9;

  nanosleep(&ts, NULL);
}

Duration benchmark(const double desired_duration, std::function<void(const double)> custom_sleep_function, const bool stress = false)
{
  pid_t child_pid = fork();

  if (child_pid == 0)
  {
    if (stress)
    {
      // TODO(@renn0xtek9): use std::system instead of execl
      // TODO(@renn0xtek9): qualify and validate the stress command !
      execl("/usr/bin/stress", "stress", "--cpu", "20", "--timeout", "10", (char *)nullptr);
    }
    exit(0);
  }
  else
  {
    // Parent process
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give the child process time to start
    TimePoint start_time_point{};
    custom_sleep_function(desired_duration);
    TimePoint end_time_point{};
    Duration duration(start_time_point, end_time_point);
    display_duration(duration, desired_duration);

    kill(child_pid, SIGTERM);
    waitpid(child_pid, nullptr, 0);
    return duration;
  }
  return {{}, {}};
}

double get_cpu_load()
{
  std::ifstream fileStat("/proc/stat");
  char cpu;
  std::string tmp;
  long double a[10];

  fileStat >> tmp;
  for (int i = 0; i < 10; ++i)
  {
    fileStat >> a[i];
  }
  if (a[3] < 0.00000001)
  {
    return -1.0; // idle cpu takes all time
  }
  return ((a[0] + a[1] + a[2]) / a[3]) * 100;
}

int main()
{
  printf("Note: CPU frequency (GHz): %f -- Clock ticks per seconds %ld -- CPU load %f\n", cpu_frequency / 1.0e9, clock_ticks_per_seconds, get_cpu_load());

  const auto desired_duration{1.0e-2};

  std::vector<BenchmarkCondition> benchmark_conditions{
      BenchmarkCondition("rtdsc", desired_duration, with_rtdsc_sleep),
      BenchmarkCondition("CPU TIME ID", desired_duration, with_clock_cpu_process_time_id),
      BenchmarkCondition("REALTIME", desired_duration, with_clock_realtime),
      BenchmarkCondition("CPU TIME ID + mm pause", desired_duration, with_clock_cpu_process_time_id_and_mm_pause),
      BenchmarkCondition("REALTIME + mm pause", desired_duration, with_clock_realtime_and_mm_pause),
      BenchmarkCondition("sleep for", desired_duration, with_sleep_for),
      BenchmarkCondition("usleep", desired_duration, with_usleep),
      BenchmarkCondition("nanosleep", desired_duration, with_nanosleep),
      BenchmarkCondition("rtdsc (stressed)", desired_duration, with_rtdsc_sleep, true),
      BenchmarkCondition("CPU TIME ID (stressed)", desired_duration, with_clock_cpu_process_time_id, true),
      BenchmarkCondition("REALTIME (stressed)", desired_duration, with_clock_realtime, true),
      BenchmarkCondition("CPU TIME ID + mm pause (stressed)", desired_duration, with_clock_cpu_process_time_id_and_mm_pause, true),
      BenchmarkCondition("REALTIME + mm pause (stressed)", desired_duration, with_clock_realtime_and_mm_pause, true),
      BenchmarkCondition("sleep for (stressed)", desired_duration, with_sleep_for, true),
      BenchmarkCondition("usleep (stressed)", desired_duration, with_usleep, true),
      BenchmarkCondition("nanosleep (stressed)", desired_duration, with_nanosleep, true)};

  std::map<std::string, Duration> results{};
  for (const auto &benchmark_condition : benchmark_conditions)
  {
    printf("\nSleep function: %s \n", benchmark_condition.name_.c_str());
    results.emplace(benchmark_condition.name_, benchmark(benchmark_condition.desired_duration_, benchmark_condition.custom_sleep_function_, benchmark_condition.stress_system_));
  }

  std::ofstream output_file("sleep-benchmark-file.csv");
  output_file << "Sleep function;Desired;CPU ticks;high_resolution_clock::now;system_clock;CLOCK_MONOTONIC;CLOCK_REALTIME;CLOCK_PROCESS_CPU_TIME_ID;CLOCK_THREAD_CPUTIME_ID\n";
  for (const auto &result : results)
  {
    output_file << result.first << ";";
    output_file << desired_duration << ";";
    output_file << result.second.rtdsc_elapsed_time << ";";
    output_file << std::chrono::duration_cast<std::chrono::duration<double>>(result.second.high_resolution_clock_duration).count() << ";";
    output_file << std::chrono::duration_cast<std::chrono::duration<double>>(result.second.system_clock_duration).count() << ";";
    output_file << static_cast<double>(result.second.clock_monotonic_duration) / 1.0e9 << ";";
    output_file << static_cast<double>(result.second.clock_realtime_duration) / 1.0e9 << ";";
    output_file << static_cast<double>(result.second.clock_process_cputime_id_duration) / 1.0e9 << ";";
    output_file << static_cast<double>(result.second.clock_thread_cputime_id_duration) / 1.0e9;
    output_file << "\n";
  }
  output_file.close();
  printf("Results saved in sleep-benchmark-file.csv\n");

  if (std::system("./plot_results.gp") != 0)
  {
    std::cerr << "Error: Unable to run gnuplot script" << std::endl;
    return 1;
  };

  return 0;
}
