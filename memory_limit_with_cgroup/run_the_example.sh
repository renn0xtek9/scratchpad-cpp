#!/bin/bash 
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"/..
CGROUPNAME="scratchpad"
BYTES="1048576" # 1MB
echo "This will create the cgroup $CGROUPNAME and set its maximum memory limit to $BYTES bytes"
sudo cgcreate -g memory:"$CGROUPNAME"
echo "$BYTES" | sudo tee /sys/fs/cgroup/memory/"$CGROUPNAME"/memory.limit_in_bytes
echo "$BYTES" | sudo tee /sys/fs/cgroup/memory/"$CGROUPNAME"/memory.kmem.limit_in_bytes
echo "$BYTES" | sudo tee /sys/fs/cgroup/memory/"$CGROUPNAME"/memory.memsw.limit_in_bytes

# Give ownership to user to allow starting cgexex for this group without sudo
sudo chown -R "$USER" /sys/fs/cgroup/memory/"$CGROUPNAME"

# Build the executable
cmake -B build 
cmake --build build

# Runit with cgexec to enforce the memory limit
cgexec -g memory:scratchpad $(pwd)/build/memory_limit_with_cgroup/memory_limit_cgroup
