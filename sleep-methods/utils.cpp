#include <fstream>
#include <thread>
#include <cmath>
#include <filesystem>

#include <x86intrin.h>
#include <unistd.h>
#include <wait.h>
#include <iostream>
#include "utils.h"
#include "types.h"
#include <limits.h>
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

bool assert_cpu_load_high()
{
    const auto cpu_load = get_cpu_load();
    if (cpu_load < MAX_CPU_LOAD_FOR_NON_STRESSED)
    {
        std::cerr << "Error: CPU load is too low ( " << cpu_load << "%), stress command may not be running" << std::endl;
        return false;
    }
    else
    {
        std::cout << "CPU load: " << cpu_load << "%" << std::endl;
    }
    return true;
}
bool assert_cpu_load_low()
{
    const auto cpu_load = get_cpu_load();
    if (cpu_load > MIN_CPU_LOAD_FOR_STRESSED)
    {
        std::cerr << "Error: CPU load is too high ( " << cpu_load << "%), stress command still may running" << std::endl;
        return false;
    }
    else
    {
        std::cout << "CPU load: " << cpu_load << "%" << std::endl;
    }
    return true;
}

Duration benchmark(const double desired_duration, std::function<void(const double)> custom_sleep_function, const bool stress)
{
    pid_t child_pid = fork();

    if (child_pid == 0)
    {
        if (stress)
        {
            execl("/usr/bin/stress", "/usr/bin/stress", "--cpu", "20", "--timeout", "10", (char *)nullptr);
        }
        exit(0);
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Give the child process time to start
        // Parent process
        if (stress)
        {

            if (!assert_cpu_load_high())
            {
                kill(child_pid, SIGKILL);
                waitpid(child_pid, nullptr, 0);
                exit(1);
                return {{}, {}};
            }
        }
        else
        {
            if (!assert_cpu_load_low())
            {
                exit(1);
            };
        }
        TimePoint start_time_point{};
        custom_sleep_function(desired_duration);
        TimePoint end_time_point{};
        Duration duration(start_time_point, end_time_point);
        display_duration(duration, desired_duration);

        int ret = std::system("pkill -9 stress"); // Somehow stress spawn lot of subprocess detached from the child_pid.
        kill(child_pid, SIGKILL);
        waitpid(child_pid, nullptr, 0);
        return duration;
    }
    return {{}, {}};
}

CpuTimes get_cpu_times()
{
    std::ifstream proc_stat("/proc/stat");
    std::string cpu;
    CpuTimes times;

    if (proc_stat.is_open() && proc_stat.good())
    {
        proc_stat >> cpu;
        for (int i = 0; i < 10; ++i)
        {
            std::uint64_t time;
            proc_stat >> time;
            times.total += time;
            if (i == 3)
            { // idle times
                times.idle = time;
            }
        }
    }
    return times;
}

double get_cpu_load()
{
    CpuTimes times_begin = get_cpu_times();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // sleep 0.1 seconds
    CpuTimes times_end = get_cpu_times();

    auto idle_time_diff = times_end.idle - times_begin.idle;
    auto total_time_diff = times_end.total - times_begin.total;

    if (total_time_diff == 0)
    {
        return -1.0; // Error: total time didn't change.
    }

    return 100.0 * (total_time_diff - idle_time_diff) / total_time_diff; // return as a percentage
}

std::filesystem::path get_path_of_current_executable()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string full_path = std::string(result, (count > 0) ? count : 0);
    return std::filesystem::path(full_path);
}
