#include "threading.h"

int increment_counter(SemaphoreHandle_t semaphore, int *counter, const char *source_thread, TickType_t timeout) {
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;
    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", source_thread, *counter);
    }
    xSemaphoreGive(semaphore);
    return pdTRUE;
}