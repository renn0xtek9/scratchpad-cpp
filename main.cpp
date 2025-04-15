#include <x86intrin.h>
#include <fstream>
#include <chrono>

/// This scratchpad shows how to make a high resolution (subm-microsecond accuracy busy wait).
/// It uses the __rdtsc() instruction to get the current CPU ticks.
/// That is, it bypasses the kernel clock. This is required to measure time accurately.
/// The CPU frequency is obtained from the /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq file.
/// The CPU frequency is in KHz, so we need to convert it to Hz.
/// The __rdtsc() is only reset every ~194 years, so we are safe...

/// For matter of comparison, we output the elapsed time that we get from the kernel clock.
/// This is not accurate, but it is useful to see how much we are off.

double getCpuFrequency()
{
  unsigned int cpuFreq;
  std::ifstream cpuinfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
  if (cpuinfo)
  {
    cpuinfo >> cpuFreq;
    cpuinfo.close();
  }
  else
  {
    throw std::runtime_error("Unable to open cpuinfo_max_freq");
  }
  return cpuFreq * 1.0e3; // convert from KHz to Hz
}

double ticksToTime(const unsigned long long &ticks, const double cpuFrequency)
{
  return ticks / cpuFrequency;
}

void high_resolution_sleep(double seconds)
{
  bool finished{false};
  auto start = __rdtsc();
  while (!finished)
  {
    auto now = __rdtsc();
    if (ticksToTime(now - start, getCpuFrequency()) >= seconds)
    {
      return;
    }
  }
}

void benchmark(const double desired_sleep_time)
{
  const double cpuFrequency = getCpuFrequency();

  const auto chrono_start_time = std::chrono::high_resolution_clock::now();

  const auto start = __rdtsc();

  high_resolution_sleep(desired_sleep_time);

  const auto end = __rdtsc();

  auto high_resolution_delat_t_ms = ticksToTime(end - start, cpuFrequency);

  const auto chrono_end_time = std::chrono::high_resolution_clock::now();

  const auto delta_t_chrono = chrono_end_time - chrono_start_time;
  const double delta_t_chrono_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(delta_t_chrono).count();
  printf("elapsed CPU tick = %llu; duration according CPU ticks %f; duration according high_resolution_clock::now %f\n", end - start, high_resolution_delat_t_ms, delta_t_chrono_seconds);
}

int main()
{

  benchmark(1.0e-3);

  return 0;
}
