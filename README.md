*This project has been created as part of the 42 curriculum by kbito.*

# Codexion

## Description

Codexion is a multithreaded resource-allocation simulation written in C.

Each coder is represented by a POSIX thread. Coders repeatedly acquire two shared USB dongles, compile, debug, and refactor. A coder burns out if they do not begin compiling before their burnout deadline.

The project focuses on:

- POSIX threads and synchronization
- Shared-resource allocation
- FIFO and EDF scheduling
- Binary heap priority queues
- Deadlock and starvation prevention
- Precise timing and burnout detection
- Thread-safe logging

A separate monitor thread observes coder deadlines and stops the simulation when a coder burns out or when every coder has completed the required number of compilations.

## Instructions

### Compilation

Compile the program with:

```sh
make
```

The executable is named:

```text
codexion
```

Other available Makefile targets are:

```sh
make clean
make fclean
make re
make debug
```

### Execution

Run the program with:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

The scheduler must be either:

```text
fifo
edf
```

Example:

```sh
./codexion 4 1250 500 100 100 3 100 edf
```

### Arguments

- `number_of_coders`: Number of coder threads and dongles.
- `time_to_burnout`: Maximum time in milliseconds between compile starts.
- `time_to_compile`: Time spent compiling while holding two dongles.
- `time_to_debug`: Time spent debugging.
- `time_to_refactor`: Time spent refactoring.
- `number_of_compiles_required`: Required compilations for every coder.
- `dongle_cooldown`: Time before a released dongle can be used again.
- `scheduler`: Dongle arbitration policy, either `fifo` or `edf`.

Negative numbers, non-integer values, missing arguments, and unsupported scheduler names are rejected.

## Scheduling policies

### FIFO

FIFO orders requests by arrival sequence. When requests compete for the same dongle, the request that arrived first has higher priority.

### EDF

EDF uses the coder's absolute burnout deadline as its primary priority:

```text
last_compile_start + time_to_burnout
```

A smaller deadline has higher priority.

When deadlines are equal, the request sequence number is used as a deterministic tie-breaker. Requests that do not compete for the same dongles may still compile concurrently.

### Priority queue

Each dongle maintains pending requests in a binary min-heap.

The comparison keys are:

```text
FIFO: request sequence number
EDF:  burnout deadline, then request sequence number
```

The heap manages request priority. The scheduler separately checks dongle availability, cooldown state, and whether both required dongles can be allocated together.

## Blocking cases handled

### Deadlock prevention

A coder never keeps one dongle while waiting indefinitely for the second one.

Allocation is handled as a single scheduler operation: a coder either acquires both required dongles or acquires neither. This removes the hold-and-wait condition and prevents circular dongle ownership.

A shared scheduler mutex serializes the decision that checks and updates the state of both dongles.

### Starvation prevention

Pending requests are retained in priority queues instead of relying only on operating-system thread scheduling.

FIFO preserves request arrival order. EDF prioritizes requests according to burnout deadlines, with a deterministic request-number tie-breaker.

Condition-variable broadcasts wake waiting coders whenever allocation-relevant state changes.

### Dongle cooldown

Every dongle stores an absolute cooldown deadline.

After a coder releases its two dongles, neither dongle can be allocated again until its cooldown has expired. Waiting coders use timed condition-variable waits rather than holding a dongle during cooldown.

### Single-coder case

When only one coder exists, only one dongle exists. The coder takes that dongle but cannot start compiling because compilation requires two dongles. The monitor eventually detects the burnout.

### Precise burnout detection

A dedicated monitor thread tracks the nearest active burnout deadline.

The monitor uses a timed condition-variable wait and rechecks coder state after waking. Timing is measured with a monotonic clock so that system clock adjustments do not change simulation deadlines.

### Serialized logging

A dedicated log mutex protects output. This prevents messages from different threads from being mixed on the same line.

After the simulation ends, ordinary action messages are suppressed. The burnout message remains printable as the final simulation event.

## Thread synchronization mechanisms

### Dongle mutexes

Each dongle has mutex-protected state, including:

- Whether the dongle is currently in use
- Its cooldown deadline

These mutexes prevent simultaneous conflicting updates to the same dongle.

### Scheduler mutex and condition variable

The scheduler mutex protects operations involving:

- Request heaps
- Eligibility checks
- Two-dongle allocation
- Dongle release
- Scheduler wait conditions

Coders that cannot acquire both dongles wait on the scheduler condition variable. Releasing dongles or changing scheduler state broadcasts this condition so waiting coders can recheck the allocation predicate.

The predicate is always checked again after waking because condition variables may wake spuriously.

### Data mutex and state condition variable

The shared data mutex protects global simulation state, including:

- Start synchronization
- The simulation-ended flag
- The number of ready coder threads
- The number of finished coder threads
- Request sequence allocation

The state condition variable coordinates the startup barrier and notifies the monitor when relevant global state changes.

### Coder mutexes

Each coder has a mutex protecting its individual mutable state:

- Burnout deadline
- Completed compilation count

This prevents the monitor from reading a partially updated coder state.

### Per-coder sleep condition variables

Each coder has its own condition variable for interruptible action waits.

Compile, debug, and refactor waits can therefore end early when the monitor stops the simulation. Per-coder condition variables also avoid unnecessary contention between unrelated coder threads.

### Monitor communication

The monitor sets the shared termination state and broadcasts all relevant condition variables.

This wakes coders waiting for:

- Simulation start
- Dongle allocation
- Dongle cooldown
- Compile, debug, or refactor completion

All waiting loops recheck their associated predicate after waking.

## Resources

The following references were used while studying and implementing the project:

- [POSIX Threads overview](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [pthread_cond_wait and pthread_cond_timedwait](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html)
- [clock_gettime](https://man7.org/linux/man-pages/man2/clock_gettime.2.html)
- [Earliest Deadline First scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Deadlock and Coffman's conditions](https://en.wikipedia.org/wiki/Deadlock)
- [min_heap ](https://www.geeksforgeeks.org/c/c-program-to-implement-min-heap/)
- [min_heap 1](https://www.w3resource.com/c-programming-exercises/heap/c-heap-exercises-3.php)
- [マルチスレッド・プログラミング(1)/ Multithread programming (1)](https://www.cs.tsukuba.ac.jp/~yas/cs/csys-2021/2021-04-16/)
- Codexion subject, version 1.5
- The 42 Norm, version 4.1

### Use of AI

AI was used only as a learning aid for:

- Understanding POSIX mutexes and condition variables
- Studying race conditions, deadlocks, lost wakeups, and starvation
- Learning how priority queues, FIFO, and EDF scheduling work
- Understanding debugging tools and concurrency-testing methods

AI was not used to generate or implement the submitted source code. All implementation decisions were made by the author after checking the subject requirements and confirming the program's behavior through compilation, testing, and debugging.
