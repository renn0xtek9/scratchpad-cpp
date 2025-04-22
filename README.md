# Enforcing memory limit of process with cgroup

In this example (see [run_the_example.sh](run_the_example.sh)) we create a `cgroup` call scratchpad.
The memory limit of processes belonging to this cgroup is set to 1048576 bytes.
The user the start the process like this

`cgexec -g memory:scratchpad /full/path/to/executable`

The process (see [main.cpp](main.cpp)) is trying to allocated a `std::vector` byte by byte until the operating system kills him

**To be improved** in this simple exampe we give ownership to user to /sys/fs/cgroup/memory/"$CGROUPNAME". This is done to not require sudo to execute cgexec -g memory:scratchpad ....
As a side effect the user can then changes the memory limit by himself.
