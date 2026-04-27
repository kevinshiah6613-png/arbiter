# Arbiter

A C++ simulation of a hardware bus arbiter — multiple hosts competing for a shared resource, resolved by three different arbitration algorithms.

## Algorithms

**Priority-Based**
The highest-priority host gets access. After being granted, its priority decreases by 1 to prevent starvation. Resets when all priorities drop below 0.

**Round Robin**
Hosts take turns in order. Each slot rotates to the next host, skipping those without a pending request.

**First Come First Served (FCFS)**
The host that sent its request earliest gets access first.

## How to compile and run

```bash
cd test
g++ main.cpp -o arbiter
./arbiter
```

## Output

All three algorithms run on the same randomly generated request schedule, followed by statistics showing how many times each host was granted or denied access — making it easy to compare the behaviour of each algorithm.
