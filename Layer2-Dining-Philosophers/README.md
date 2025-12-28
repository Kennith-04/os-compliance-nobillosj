# Layer 2: Dining Philosophers Implementation

## Problem: Circular Wait
The classic Dining Philosophers problem illustrates a common synchronization issue called **Deadlock**. This occurs when every philosopher holds one resource (the left fork) and waits indefinitely for a resource held by their neighbor (the right fork). This creates a "circular wait" condition where no progress can be made.

## Solution: Resource Hierarchy
To prevent deadlock, I implemented the **Resource Hierarchy** (also known as the "Numbered Forks") strategy. This strategy imposes a strict ordering on resource acquisition to break the circular dependency.

### The Algorithm
I assign a unique number (ID) to each fork (0 through 4). The rule is simple: **A philosopher must always acquire the lower-numbered fork before the higher-numbered fork.**



### Implementation Details
**Philosophers 0–3:**
* **Left Fork:** `i`
* **Right Fork:** `(i + 1) % 5`
* **Action:** Since `Left < Right`, they pick up the **Left** fork first, then the **Right** fork.

**Philosopher 4 (The Tie-Breaker):**
* **Left Fork:** `4`
* **Right Fork:** `0`
* **Action:** Since `Right (0) < Left (4)`, Philosopher 4 picks up the **Right** fork first, then the **Left** fork.

### Why This Prevents Deadlock
By forcing the last philosopher to pick up forks in the reverse order of their neighbors, we mathematically eliminate the possibility of a cycle.

If all philosophers become hungry simultaneously:
1.  Philosophers 0–3 attempt to pick up their left forks.
2.  Philosopher 4 attempts to pick up their right fork (Fork 0).
3.  Philosopher 4 competes with Philosopher 0 for Fork 0.
4.  One of them will lose and wait. This prevents the circle from closing, ensuring at least one philosopher can acquire both forks to eat.

---

## Compilation and Execution

### Prerequisites
* **OS:** Linux or Windows Subsystem for Linux (WSL)
* **Compiler:** GCC
* **Libraries:** `pthread` (POSIX Threads)

### Compilation
Open the Terminal
Use the included Makefile or compile manually with `gcc`:

```bash
gcc philosophers.c -o dining -pthread:

./dining.

```
Long-Duration Test (Deadlock Verification)
##To verify that no deadlock occurs over a long period:
Allow it to run for 3–5 minutes.
Observe that the output continues scrolling and does not freeze.
Press Ctrl + C to terminate the program.

