#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // Number of philosophers

// Semaphores: One for each chopstick (fork)
sem_t chopsticks[N];

void* philosopher(void* num) {
    int id = *(int*)num;

    // --- DEADLOCK PREVENTION STRATEGY: RESOURCE HIERARCHY ---
    // Rule: Always pick up the lower-numbered fork first.
    //
    // For philosophers 0-3: Left fork is lower ID.
    // For philosopher 4: Right fork (0) is lower ID than Left fork (4).
    
    int first_fork = id;
    int second_fork = (id + 1) % N;
    
    if (id == N - 1) {
        // Swap for the last philosopher so they pick up 0 first
        int temp = first_fork;
        first_fork = second_fork;
        second_fork = temp;
    }

    // Loop a few times to demonstrate it works
    // (You can make this an infinite loop 'while(1)' if you prefer)
    for(int i = 0; i < 3; i++) { 
        
        printf("Philosopher %d is thinking...\n", id);
        sleep(1); // Simulate thinking

        // CRITICAL SECTION: PICKING UP FORKS
        printf("Philosopher %d is waiting for fork %d\n", id, first_fork);
        sem_wait(&chopsticks[first_fork]); // Lock first fork
        
        printf("Philosopher %d is waiting for fork %d\n", id, second_fork);
        sem_wait(&chopsticks[second_fork]); // Lock second fork

        // EATING
        printf("Philosopher %d is EATING\n", id);
        sleep(1); // Simulate eating time

        // PUTTING DOWN FORKS
        sem_post(&chopsticks[second_fork]);
        sem_post(&chopsticks[first_fork]);

        printf("Philosopher %d finished eating round %d.\n", id, i+1);
    }
    return NULL;
}

int main() {
    pthread_t thread_id[N];
    int phil_ids[N];

    // 1. Initialize Semaphores
    for (int i = 0; i < N; i++) {
        // sem_init(semaphore_pointer, 0=shared_between_threads, initial_value)
        sem_init(&chopsticks[i], 0, 1);
        phil_ids[i] = i;
    }

    // 2. Create Threads
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil_ids[i]);
    }

    // 3. Wait for Threads (Join)
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // 4. Cleanup
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}