# Arbiter

A C++ simulation of a hardware bus arbiter — three hosts competing for a shared resource, resolved by a priority-based arbitration algorithm.

## What it does

- Three hosts each have an ID and a priority value
- At each time slot, hosts randomly send requests for the shared bus
- The arbiter grants access to the highest-priority host that has a pending request
- After being granted access, a host's priority decreases by 1 (to prevent starvation)
- When all priorities drop below 0, they reset to 0

## How to compile and run

```bash
cd test
g++ main.cpp -o arbiter
./arbiter
```

## Project structure

```
test/        # Arbiter main simulation (C++)
orange/      # Sorting algorithm exercise (C++)
teat/        # Swift iOS test project
```

## Background

Built as a university systems programming exercise to simulate bus arbitration logic in hardware design.
