*This project has been created as part of the 42 curriculum by asuleime.*

# Codexion: Master the race for resources before the deadline masters you

Codexion is a concurrent systems programming project developed at 42. It models a circular inclusive co-working hub where multiple coder threads compete for shared USB dongles to compile quantum code on a central quantum compiler, debug, and refactor, all while avoiding burnout under strict real-time deadline constraints and fair arbitration policies (FIFO and EDF).

---

## Table of Contents
1. [Description](#description)
2. [Instructions](#instructions)
3. [Resources & AI Usage](#resources--ai-usage)
4. [Blocking Cases Handled](#blocking-cases-handled)
5. [Thread Synchronization Mechanisms](#thread-synchronization-mechanisms)

---

## Description

The simulation takes place in a circular co-working space where $N$ coders sit around a table with $N$ USB dongles placed between each adjacent pair. To compile quantum code, a coder requires two dongles simultaneously (their left and right dongles).

Once compilation completes, the coder releases both dongles back onto the table, placing them into a mandatory hardware cooldown period (`dongle_cooldown`), and immediately transitions into debugging. After debugging, the coder enters a refactoring phase. Upon completing refactoring, the coder immediately attempts to acquire dongles to compile again.

Every coder must compile regularly. If a coder fails to start compiling within `time_to_burnout` milliseconds since the start of their last compilation (or since the beginning of the simulation), they burn out and the simulation halts.

Dongle acquisition conflicts are arbitrated using a custom **Binary Min-Heap Priority Queue** implementing two scheduling disciplines:
- **FIFO (First-In, First-Out)**: Dongles are awarded in order of request arrival timestamps.
- **EDF (Earliest Deadline First)**: Dongles are awarded to the coder with the earliest burnout deadline (`last_compile_start + time_to_burnout`).
 Lower coder ID (`coder_num` field value) is the deterministic tie-breaker.

---

## Instructions

### Compilation

The project compiles with `cc` using `-Wall -Wextra -Werror -pthread`:

```bash
# Standard compilation
make

# AddressSanitizer build (detects memory leaks, use-after-free, buffer overflows)
make asan

# ThreadSanitizer build (compiler-level data race detection)
make tsan

# Valgrind debug symbols build
make valgrind

# Clean object files (1), plus binaries (2), and make again (3)
make clean
make fclean
make re
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

#### Arguments:
Other than `number_of_coders` and `scheduler`, all arguments are within the 32-bit unsigned integer range.
- `number_of_coders`: Number of coders and dongles (1 to 300).
- `time_to_burnout`: Milliseconds before a coder burns out if not compiling.
- `time_to_compile`: Milliseconds spent compiling (holding two dongles).
- `time_to_debug`: Milliseconds spent debugging.
- `time_to_refactor`: Milliseconds spent refactoring.
- `number_of_compiles_required`: If all coders compile this many times, simulation stops (set to `0` to run until burnout).
- `dongle_cooldown`: Milliseconds a dongle remains unavailable after being released.
- `scheduler`: Arbitration algorithm (`fifo` or `edf`).

#### Example Commands:
```bash
# 4 coders, 410ms burnout, EDF arbitration, 2 compiles each:
./codexion 4 410 200 100 100 2 0 edf

# 4 coders with 50ms dongle cooldown:
./codexion 4 600 150 100 100 2 50 edf

# 1 coder edge case (burnout expected):
./codexion 1 300 100 100 100 0 0 fifo
```

---

## Resources & AI Usage

### References
- *Multithreaded Programming (POSIX pthreads Tutorial)* at [randu.org](https://randu.org/tutorials/threads/).
- *Operating Systems: Three Easy Pieces* (Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau) — Concurrency, Condition Variables, Semaphores, and Deadlock.
- *POSIX Threads Programming* (Lawrence Livermore National Laboratory tutorial).
- *The Linux Programming Interface* (Michael Kerrisk) — POSIX Threads and Real-Time Signals.
- *Introduction to Algorithms* (Cormen, Leiserson, Rivest, Stein) — Binary Heaps and Priority Queues.
- Valgrind Documentation: Memcheck, Helgrind, and DRD Manuals. [[link]](https://valgrind.org/docs/manual/index.html)

### Description of AI Usage
AI assistance was utilized for:
- Code audits against 42 Norm (v4.1) constraints (verifying 25-line limits, 5-function limits, and pointer alignments).
- Explainig how AddressSanitizer, DRD, and Helgrind work and what they check.
- Diagnosing subtle data races flagged by Valgrind DRD.
- Drafting this README's headers for sections and links in the table of contents'.
- Adding log messages for makefile commands

---

## Blocking Cases Handled

1. **Deadlock Prevention (Coffman's Conditions)**:
   A classic deadlock occurs when all coders attempt to acquire their left dongle and wait for their right dongle (Circular Wait). Codexion breaks the circular wait condition using **Resource Hierarchy**:
   - Each coder determines the lower ID between their left and right dongle (`min(l_dongle->id, r_dongle->id)`) and always requests it first.
   - For coders $1 \dots N-1$, this means acquiring dongle $i$ then $i+1$.
   - For coder $N$, their dongles are $N$ and $1$. Because $1 < N$, coder $N$ requests dongle $1$ first and dongle $N$ second.
   - This strict total ordering guarantees that no circular chain of dependency can ever form.

2. **Starvation Prevention & Fair Scheduling**:
   - Coders wait in a per-dongle **Binary Min-Heap Priority Queue**.
   - Under **FIFO**, requests are served strictly in order of arrival timestamps.
   - Under **EDF**, requests are prioritized by earliest impending burnout deadline (`last_compile_start + burnout_t`). A coder close to burnout automatically jumps to the head of the dongle queue ahead of coders with ample remaining time.
   - Deterministic tie-breaking uses lower coder ID to guarantee predictable scheduling.

3. **Dongle Cooldown Handling**:
   - When a dongle is released, its `free_t` timestamp is set to `now + dongle_cooldown`.
   - Waiting coders use `pthread_cond_timedwait` with periodic 5 ms timeouts inside `dongle_wait()`. This ensures that as soon as the cooldown expires, the top-priority waiter re-evaluates `get_time_ms() >= dongle->free_t` and acquires the dongle immediately without requiring manual external signals.

4. **Precise Burnout Detection ($\le 10$ ms)**:
   - A dedicated monitor thread polls all coders every 1 ms (`usleep(1000)`).
   - Because check intervals are 1 ms, burnouts are detected within 1–2 ms of actual deadline expiration, comfortably satisfying the 10 ms precision requirement.

5. **Log Serialization**:
   - A central `log_mutex` guards all logging calls.
   - No log messages ever interleave on stdout.
   - When a burnout occurs, `is_end = true` is set while holding `log_mutex` before the burnout message is printed. Any coder waiting to log immediately checks `is_end` once acquiring the mutex and drops its log, guaranteeing that **no messages are displayed after a burnout log**.

6. **Single Coder Edge Case**:
   - When `n_coders == 1`, only 1 dongle exists on the table (`coder->r_dongle == NULL`).
   - `handle_single_coder`, called in this case, acquires the single available dongle, prints the log, and waits until the monitor thread detects burnout at `burnout_t` ms.

---

## Thread Synchronization Mechanisms

The implementation relies on POSIX thread primitives (`pthread_mutex_t`, `pthread_cond_t`) structured to guarantee thread safety and zero data races:

1. **`pthread_mutex_t mutex` (per Dongle)**:
   Protects each dongle's internal state (`in_use`, `free_t`) and its binary min-heap priority queue (`queue`). Any queue insertion (`heap_push`), removal (`heap_pop`, `heap_remove`), or inspection (`heap_peek`) is performed exclusively under this lock.

2. **`pthread_cond_t cond` (per Coder)**:
   Each coder possesses an individual condition variable. When waiting in a dongle queue, the coder waits on `&coder->cond` while holding the dongle's mutex. When the dongle is released, the releasing thread signals the condition variables of waiting coders.

3. **`pthread_mutex_t c_mutex` (per Coder)**:
   Protects shared coder state accessed concurrently by the coder and monitor threads:
   - `last_cc_t`: Updated by the coder upon starting compilation; read by the monitor to evaluate burnout.
   - `cc_count`: Incremented by the coder after compiling; read by the monitor to check simulation completion.

4. **`pthread_mutex_t end_mutex` & `bool is_end`**:
   Guarantees memory visibility and atomic state transitions when the simulation halts. Every read of `is_end` is wrapped in `is_simulation_over()`, preventing compiler reordering and data races.

5. **`pthread_mutex_t log_mutex`**:
   Ensures mutual exclusion for stdout, serializing terminal writes across all threads.

### Note on Helgrind warning: "...dubious: associated lock is not held by any thread"

This error warning is caused by the use of `pthread_cond_timedwait()` by coders when waiting for a dongle to become available. The Valgrind [docs](https://valgrind.org/docs/manual/hg-manual.html#hg-manual.api-checks) mentions the following:

"Signalling or broadcasting a condition variable when the associated mutex is unlocked is not strictly an error. The resulting thread scheduling may be unpredictable if the mutex is not held. The option --check-cond-signal-mutex=yes|no turns on checking for this situation. This kind of error is categorised as 'dubious'. The check is not turned on by default because some standard C and C++ libraries use condition signals/broadcasts with the associated mutex unlocked."

In this implementation of Codexion, this issue is caused by 