#ifndef TYPES_H
#define TYPES_H
#include <chrono>
#include <x86intrin.h>
#include <functional>

#define BILLION 1000000000L

constexpr double MAX_CPU_LOAD_FOR_NON_STRESSED{13.0};
constexpr double MIN_CPU_LOAD_FOR_STRESSED{90.0};

const double cpu_frequency{2.7e9}; // according /proc/cpuinfo
inline double cpuTicksToTime(const unsigned long long &cpu_ticks)
{
    // Very not sure about that !
    return cpu_ticks / cpu_frequency;
}

inline uint64_t durationBetweenTimespecs(const timespec &start, const timespec &end)
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

#endif
