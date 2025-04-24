#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include "types.h"
double get_cpu_load();
double getCpuMaxFrequency();
void display_duration(const Duration &duration, const double desired_duration);
void with_rtdsc_sleep(const double seconds);
void polling_clock(const double seconds, const clockid_t clock_id);
void polling_clock_and_mm_pause(const double seconds, const clockid_t clock_id);
void with_clock_cpu_process_time_id(const double seconds);
void with_clock_realtime(const double seconds);
void with_clock_cpu_process_time_id_and_mm_pause(const double seconds);
void with_clock_realtime_and_mm_pause(const double seconds);
void with_sleep_for(const double seconds);
void with_usleep(const double seconds);
void with_nanosleep(const double seconds);
Duration benchmark(const double desired_duration, std::function<void(const double)> custom_sleep_function, const bool stress = false);

#endif
