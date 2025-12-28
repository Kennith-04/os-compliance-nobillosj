#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS 2
#define CONSUMERS 3
#define ITEMS_TO_PRODUCE 10 // Total items each producer creates

// --- Shared Buffer Structure ---
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;              // Index for next write
    int out;             // Index for next read
    sem_t empty;         // Counts empty slots
    sem_t full;          // Counts full slots
    sem_t mutex;         // Binary semaphore for lock
} buffer_t;

buffer_t shared_buf;

// Helper to print buffer state (Must be called inside critical section)
void print_buffer_state() {
    printf(" | Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == shared_buf.in && i == shared_buf.out) printf(" IO "); // In/Out overlap
        else if (i == shared_buf.in) printf(" I ");
        else if (i == shared_buf.out) printf(" O ");
        else printf(" %d ", shared_buf.buffer[i]);
    }
    printf("]\n");
}

// --- Producer Thread ---
void* producer(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        int item = (id * 100) + i; // Generate a unique item number
        
        sleep(rand() % 2); // Simulate time to produce data

        // 1. Wait for an empty slot
        sem_wait(&shared_buf.empty);
        // 2. Wait for exclusive access
        sem_wait(&shared_buf.mutex);

        // --- CRITICAL SECTION ---
        shared_buf.buffer[shared_buf.in] = item;
        printf("[Producer %d] Produced %d at slot %d", id, item, shared_buf.in);
        
        // Advance 'in' index circular
        shared_buf.in = (shared_buf.in + 1) % BUFFER_SIZE;
        
        print_buffer_state();
        // ------------------------

        // 3. Release exclusive access
        sem_post(&shared_buf.mutex);
        // 4. Signal that a slot is full
        sem_post(&shared_buf.full);
    }
    printf("[Producer %d] Finished producing.\n", id);
    return NULL;
}

// --- Consumer Thread ---
void* consumer(void* arg) {
    int id = *(int*)arg;
    
    while (1) { // Consumers run forever in this example
        // 1. Wait for a full slot (data available)
        sem_wait(&shared_buf.full);
        // 2. Wait for exclusive access
        sem_wait(&shared_buf.mutex);

        // --- CRITICAL SECTION ---
        int item = shared_buf.buffer[shared_buf.out];
        printf("[Consumer %d] Consumed %d from slot %d", id, item, shared_buf.out);
        
        // Clear the slot (optional, just for visual clarity)
        shared_buf.buffer[shared_buf.out] = 0;

        // Advance 'out' index circular
        shared_buf.out = (shared_buf.out + 1) % BUFFER_SIZE;
        
        print_buffer_state();
        // ------------------------

        // 3. Release exclusive access
        sem_post(&shared_buf.mutex);
        // 4. Signal that a slot is empty
        sem_post(&shared_buf.empty);

        sleep(1 + (rand() % 3)); // Simulate time to process data
    }
    return NULL;
}

int main() {
    pthread_t prod_threads[PRODUCERS];
    pthread_t cons_threads[CONSUMERS];
    int prod_ids[PRODUCERS];
    int cons_ids[CONSUMERS];

    // 1. Initialize Semaphores & Buffer indices
    sem_init(&shared_buf.empty, 0, BUFFER_SIZE); // Starts with 5 empty slots
    sem_init(&shared_buf.full, 0, 0);            // Starts with 0 full slots
    sem_init(&shared_buf.mutex, 0, 1);           // Starts unlocked (1)
    shared_buf.in = 0;
    shared_buf.out = 0;

    // Initialize buffer with zeros for cleaner printing
    for(int i=0; i<BUFFER_SIZE; i++) shared_buf.buffer[i] = 0;

    // 2. Create Consumers (They will wait initially because 'full' is 0)
    for (int i = 0; i < CONSUMERS; i++) {
        cons_ids[i] = i;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_ids[i]);
    }

    // 3. Create Producers
    for (int i = 0; i < PRODUCERS; i++) {
        prod_ids[i] = i;
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }

    // 4. Wait for Producers to finish
    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }

    // Note: In a real system, we would signal consumers to stop. 
    // Here, we just let the main program exit, which kills the infinite consumer threads.
    printf("All producers finished. Exiting.\n");
    
    sem_destroy(&shared_buf.empty);
    sem_destroy(&shared_buf.full);
    sem_destroy(&shared_buf.mutex);

    return 0;
}