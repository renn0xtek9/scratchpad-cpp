

#include <fstream>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <map>
#include <iostream>

#include <immintrin.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "types.h"
#include "utils.h"
#include "definitions.h"

const long int clock_ticks_per_seconds{sysconf(_SC_CLK_TCK)}; // Don't confuse with cpu ticks !

void benchmark_all_methods_for_sleep_time_nominal()
{
  const auto desired_duration{SLEEP_TIME_NOMINAL};

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
    std::cout << "Sleep function: " << benchmark_condition.name_ << std::endl;
    results.emplace(benchmark_condition.name_, benchmark(benchmark_condition.desired_duration_, benchmark_condition.custom_sleep_function_, benchmark_condition.stress_system_));
  }

  const auto executable_path = std::filesystem::path(get_path_of_current_executable());
  const std::filesystem::path csv_path = executable_path.parent_path().parent_path() / "share" / "sleep-methods" / "sleep-benchmark-file.csv";

  std::ofstream output_file(csv_path.string().c_str());
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
  std::cout << "Results saved in " << csv_path.string() << std::endl;
}

void benchmark_clock_real_time_distribution(const std::string csv_filename, const bool stress)
{
  const auto desired_duration{SLEEP_TIME_NOMINAL};
  const std::size_t result_size{100U};
  BenchmarkCondition benchmark_condition("REALTIME + mm pause (stressed)", desired_duration, with_clock_realtime_and_mm_pause, stress);
  std::vector<Duration> results{};
  for (int i = 0; i < result_size; i++)
  {
    results.push_back(benchmark(benchmark_condition.desired_duration_, benchmark_condition.custom_sleep_function_, benchmark_condition.stress_system_));
  }

  const auto executable_path = std::filesystem::path(get_path_of_current_executable());
  const std::filesystem::path csv_path = executable_path.parent_path().parent_path() / "share" / "sleep-methods" / csv_filename.c_str();

  std::ofstream output_file(csv_path.string().c_str());
  output_file << "Iteration;Duration\n";
  for (int index = 0; index < results.size(); index++)
  {
    output_file << index << ";";
    output_file << std::abs(desired_duration - static_cast<double>(results.at(index).clock_realtime_duration) / 1.0e9) * 1.0e6 << ";";
    output_file << std::endl;
  }
  output_file.close();
  std::cout << "Results saved in " << csv_path.string() << std::endl;
}

void run_gnuplot()
{
  const auto executable_path = std::filesystem::path(get_path_of_current_executable());
  auto gnuplot_script = executable_path.parent_path().parent_path() / "share" / "sleep-methods" / "plot_results.gp";

  if (chdir(gnuplot_script.parent_path().c_str()) != 0)
  {
    std::cerr << "Permission denied: " << gnuplot_script.parent_path().string() << std::endl;
  }
  if (std::system(gnuplot_script.string().c_str()) != 0)
  {
    std::cerr << "Error: Unable to run gnuplot script" << std::endl;
    return;
  };
}

int main()
{
  std::cout << "Note: CPU frequency (GHz): " << cpu_frequency / 1.0e9 << " -- Clock ticks per seconds " << clock_ticks_per_seconds << " -- CPU load " << get_cpu_load() << std::endl;

  benchmark_all_methods_for_sleep_time_nominal();
  benchmark_clock_real_time_distribution("clock_realtime_idle_delta_microseconds.csv", false);
  benchmark_clock_real_time_distribution("clock_realtime_stressed_delta_microseconds.csv", true);
  run_gnuplot();

  return 0;
}
