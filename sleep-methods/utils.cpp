#include <fstream>
#include <thread>
#include <cmath>

#include <x86intrin.h>
#include <unistd.h>
#include <wait.h>

#include "utils.h"
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

Duration benchmark(const double desired_duration, std::function<void(const double)> custom_sleep_function, const bool stress)
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
