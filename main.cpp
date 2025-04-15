#include <x86intrin.h>
#include <fstream>
#include <chrono>
#include <time.h>
#include <functional>
#include <thread>
#include <unistd.h>
#include <cmath>

#define BILLION 1000000000L

const long int clock_ticks_per_seconds{sysconf(_SC_CLK_TCK)}; // Don't confuse with cpu ticks !

double getCpuFrequency()
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

const double cpu_frequency{getCpuFrequency()}; // in Hz
double cpuTicksToTime(const unsigned long long &cpu_ticks)
{
  // Very not sure about that !
  return cpu_ticks / cpu_frequency;
}

struct TimePoint
{
  TimePoint() : system_clock_time(std::chrono::system_clock::now()),
                high_resolution_clock_time(std::chrono::high_resolution_clock::now()),
                rtdsc_time{__rdtsc()}
  {
    clock_gettime(CLOCK_MONOTONIC, &monotonic_clock_time);
  }

  struct timespec monotonic_clock_time;
  std::chrono::system_clock::time_point system_clock_time{};
  std::chrono::high_resolution_clock::time_point high_resolution_clock_time{};
  const unsigned long long rtdsc_time{};
};

struct Duration
{
  Duration(const TimePoint &start, const TimePoint &end)
      : monotonic_clock_duration(BILLION * (end.monotonic_clock_time.tv_sec - start.monotonic_clock_time.tv_sec) +
                                 end.monotonic_clock_time.tv_nsec - start.monotonic_clock_time.tv_nsec),
        system_clock_duration(end.system_clock_time - start.system_clock_time),
        high_resolution_clock_duration(end.high_resolution_clock_time - start.high_resolution_clock_time),
        rtdsc_elapsed_ticks(end.rtdsc_time - start.rtdsc_time),
        rtdsc_elapsed_time{cpuTicksToTime(rtdsc_elapsed_ticks)} {}

  uint64_t monotonic_clock_duration;
  std::chrono::nanoseconds system_clock_duration{};
  std::chrono::high_resolution_clock::duration high_resolution_clock_duration{};
  const unsigned long long rtdsc_elapsed_ticks{};
  const double rtdsc_elapsed_time{};
};

void display_duration(const Duration &duration, const double desired_duration)
{
  printf("Desired %f -- Duration according: ", desired_duration);
  printf("CPU ticks %f; ", duration.rtdsc_elapsed_time);
  printf("high_resolution_clock::now %f; ", std::chrono::duration_cast<std::chrono::duration<double>>(duration.high_resolution_clock_duration).count());
  printf("system_clock %f; ", std::chrono::duration_cast<std::chrono::duration<double>>(duration.system_clock_duration).count());
  printf("clock_get_time %f; ", static_cast<double>(duration.monotonic_clock_duration / 1.0e9));
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
  ts.tv_nsec = fracPart * 1e9;

  nanosleep(&ts, NULL);
}

void benchmark(const double desired_duration, std::function<void(const double)> custom_sleep_function)
{
  TimePoint start_time_point{};
  custom_sleep_function(desired_duration);
  TimePoint end_time_point{};
  Duration duration(start_time_point, end_time_point);
  display_duration(duration, desired_duration);
}

int main()
{
  printf("Note: CPU frequency (GHz): %f -- Clock ticks per seconds %ld\n", cpu_frequency / 1.0e9, clock_ticks_per_seconds);

  const auto desired_duration{1.0e-1};

  printf("rtdsc benchmark\n");
  benchmark(desired_duration, with_rtdsc_sleep);
  printf("sleep_for benchmark\n");
  benchmark(desired_duration, with_sleep_for);
  printf("usleep benchmark\n");
  benchmark(desired_duration, with_usleep);
  printf("nanosleep benchmark\n");
  benchmark(desired_duration, with_nanosleep);

  return 0;
}
