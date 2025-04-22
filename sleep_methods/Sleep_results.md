# Sleep technique benchmark

## Short sleep (1ms)

### Idling CPU

CPU has no other workload then IDE and terminal running in background

- polling `rtdsc` has 7us excess sleep. Probably CPU frequency is not accurate enough
- polling `CLOCK_PROCESS_CPU_ID_TIME` has 1us.
- polling `CLOCK_REALTIME` has 0.
- `std::sleep_for`has 159 us

### Stressing CPU

Using `stress` to simulate cpu load

- polling `rtdsc` has 41us excess sleep. Probably CPU frequency is not accurate enough
- polling `CLOCK_PROCESS_CPU_ID_TIME` has 18us.
- polling `CLOCK_REALTIME` has 1.
- `std::sleep_for`has 57 us

## Long sleep (10s)

### Idling CPU

CPU has no other workload then IDE and terminal running in background

- polling `CLOCK_REALTIME` seems the better option. Note that `CLOCK_PROCESS_CPU_TIME_ID` and `CLOCK_REALTIME` are drifting apart (`CLOCK_PROCESS_CPU_TIME_ID` has 194us delay). It hints that `CLOCK_PROCESS_CPU_TIME_ID` is only the time spent in the CPU and Linux scheduler may have took us some time away.
- `std::sleep_for`has 225 us excess sleep.
- polling `rtdsc` has 44.6 ms excess sleep (probably because of non constant/non accurte CPU freq.)

### Stressing CPU

Using `stress` to simulate cpu load

- polling `CLOCK_REALTIME` stays relatively good (300 us excess vs 3us if not stress)
- polling `CLOCK_PROCESS_CPU_TIME_ID` is bad (**~2s excess!!!**)
- `std::sleep_for` is better (64us excess sleep). *It is surprinsingly more exact when stressed as when not stressed*

## Conclusion

- polling `CLOCK_REALTIME` accuracy seems to stay below 1ms for all duration
- `std::sleep_for` seems to work better when CPU is not idling :-)
