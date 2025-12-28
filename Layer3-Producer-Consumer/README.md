# Layer 3: Producer-Consumer Implementation

## Synchronization Approach
This program solves the classic Producer-Consumer problem using a **Bounded Buffer** (Circular Queue) and **Semaphore-based synchronization**.



**The Strategy:**
I use a shared fixed-size buffer of 5 slots.
* **Producers** generate data and place it into the buffer. If the buffer is full, they block (wait) until a slot becomes available.
* **Consumers** remove data from the buffer. If the buffer is empty, they block (wait) until new data arrives.

To prevent race conditions (where two threads try to modify the buffer index simultaneously) and to manage resource counting, we use three specific semaphores.

## The Three Semaphores
We utilize the standard "Three-Semaphore" pattern to enforce safety and coordination:

### 1. `mutex` (Binary Semaphore)
* **Purpose:** Mutual Exclusion.
* **Usage:** It acts as a lock around the "Critical Section" (the code where items are added or removed from the buffer).
* **Behavior:** Only one thread (Producer *or* Consumer) can hold the `mutex` at any time. This prevents data corruption effectively.

### 2. `empty` (Counting Semaphore)
* **Purpose:** Tracks available empty slots for Producers.
* **Initial Value:** 5 (Size of Buffer).
* **Usage:**
    * **Producers** `wait()` on this. If it is > 0, they proceed (decrement). If 0, they sleep (buffer is full).
    * **Consumers** `post()` (signal) this after removing an item, effectively telling producers, "I made a free spot!"

### 3. `full` (Counting Semaphore)
* **Purpose:** Tracks available filled slots for Consumers.
* **Initial Value:** 0 (Buffer starts empty).
* **Usage:**
    * **Consumers** `wait()` on this. If it is > 0, they proceed (decrement). If 0, they sleep (buffer is empty).
    * **Producers** `post()` (signal) this after adding an item, telling consumers, "There is new data to eat!"

---

## Compilation and Execution

### Prerequisites
* **OS:** Linux or Windows Subsystem for Linux (WSL)
* **Compiler:** GCC
* **Libraries:** `pthread` (POSIX Threads)

### Compilation
Use the included Makefile or compile manually:

```bash
//Makefile
make
//Manually
gcc producer_consumer.c -o prodcon -pthread
./prodcon
