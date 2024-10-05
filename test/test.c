#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "unity_config.h"
#include "threading.h"

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
    

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_standard_behavior);
    RUN_TEST(test_timeout);
    sleep_ms(5000);
    return UNITY_END();
}
