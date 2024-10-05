#include "threading.h"

int increment_counter(SemaphoreHandle_t semaphore, int* counter, const char *source_thread, TickType_t timeout) {
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;
    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", source_thread, *counter);
    }
    xSemaphoreGive(semaphore);
    return pdTRUE;
}

void deadlock(void* params) {

    struct deadlock_params* deadlock = (struct deadlock_params*) params;

    printf("Deadlock thread started in thread %s\n", deadlock->source_thread);
    deadlock->counter++;

    xSemaphoreTake(deadlock->semaphore_a, portMAX_DELAY);
    {
        printf("First lock in thread %s\n", deadlock->source_thread);
        deadlock->counter++;

        // Allow second thread to catch up to cause deadlock 
        vTaskDelay(100);

        xSemaphoreTake(deadlock->semaphore_b, portMAX_DELAY);
        {
            printf("Second lock in thread %s\n", deadlock->source_thread);
            deadlock->counter++;
        }
        xSemaphoreGive(deadlock->semaphore_b);
    }
    xSemaphoreGive(deadlock->semaphore_a);

    // Thread will eventually timeout on the semaphore take and exit to here.
    vTaskSuspend(NULL);
}