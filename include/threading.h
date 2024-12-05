#pragma once

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

struct deadlock_params {
    SemaphoreHandle_t semaphore_a;
    SemaphoreHandle_t semaphore_b;
    int counter;
    const char *source_thread;
};

int increment_counter(SemaphoreHandle_t semaphore, int *counter, const char *source_thread, TickType_t timeout);

void deadlock(void* params);

int orphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *counter);

int unorphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *counter);