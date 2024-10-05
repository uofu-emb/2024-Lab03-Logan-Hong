#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "unity_config.h"
#include "threading.h"

#define TEST_RUNNER_PRIORITY ( tskIDLE_PRIORITY + 5UL )
#define LEFT_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define LEFT_TASK_PRIORITY ( TEST_RUNNER_PRIORITY - 1UL )
#define RIGHT_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define RIGHT_TASK_PRIORITY ( TEST_RUNNER_PRIORITY - 1UL )

void setUp(void) {}

void tearDown(void) {}

void test_standard_behavior()
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;

    int result = increment_counter(semaphore, &counter, "test", portMAX_DELAY);

    TEST_ASSERT_TRUE_MESSAGE(result, "increment_counter failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, counter, "counter not incremented");
}

void test_timeout()
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;

    xSemaphoreTake(semaphore, portMAX_DELAY);

    int result = increment_counter(semaphore, &counter, "test", portMAX_DELAY);

    TEST_ASSERT_FALSE_MESSAGE(result, "increment_counter succeeded");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, counter, "counter incremented");
}

void test_deadlock() {

    // Create two semaphores
    SemaphoreHandle_t semaphore_a = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t semaphore_b = xSemaphoreCreateCounting(1, 1);

    // Create two deadlock_params with opposite semaphores
    struct deadlock_params params_a = {
        .semaphore_a = semaphore_a,
        .semaphore_b = semaphore_b,
        .counter = 0,
        .source_thread = "a"
    };

    struct deadlock_params params_b = {
        .semaphore_a = semaphore_b,
        .semaphore_b = semaphore_a,
        .counter = 5,
        .source_thread = "b"
    };

    // Create two Threads
    TaskHandle_t thread_a, thread_b;
    xTaskCreate(deadlock, "DeadlockA", configMINIMAL_STACK_SIZE, &params_a, LEFT_TASK_PRIORITY + 1, &thread_a);
    xTaskCreate(deadlock, "DeadlockB", configMINIMAL_STACK_SIZE, &params_b, RIGHT_TASK_PRIORITY + 1, &thread_b);

    // Give the threads time to deadlock and timeout on the semaphore take
    // sleep_ms(10000);

    // Check that the threads are deadlocked
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore_a));
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore_b));

    // TEST_ASSERT_EQUAL_INT(2, params_a.counter);
    // TEST_ASSERT_EQUAL_INT(7, params_b.counter);

    // Clean up
    vTaskDelete(thread_a);
    vTaskDelete(thread_b);
}

void test_orphaned_lock() {
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;

    int result = orphaned_lock(semaphore, portMAX_DELAY, &counter);

    TEST_ASSERT_FALSE_MESSAGE(result, "orphaned_lock failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, counter, "incorrect counter value");
}

void test_unorphaned_lock() {
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;

    int result = unorphaned_lock(semaphore, portMAX_DELAY, &counter);

    TEST_ASSERT_TRUE_MESSAGE(result, "unorphaned_lock failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, counter, "incorrect counter value");
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_standard_behavior);
    RUN_TEST(test_timeout);
    RUN_TEST(test_deadlock);
    RUN_TEST(test_orphaned_lock);
    RUN_TEST(test_unorphaned_lock);
    sleep_ms(5000);
    return UNITY_END();
}
