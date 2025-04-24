

#include <fstream>

#include <immintrin.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>

#include "types.h"
#include "utils.h"

const long int clock_ticks_per_seconds{sysconf(_SC_CLK_TCK)}; // Don't confuse with cpu ticks !

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
