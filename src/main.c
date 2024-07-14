/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 500 msec */
#define SLEEP_TIME_MS   500
#define BLINKY_2_SLEEP_TIME_MS   250

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
/* The devicetree node identifier for the "led1" alias. */
#define LED1_NODE DT_ALIAS(led1)

#define THREAD_STACK_SIZE_BLINKY_2	1024
#define THREAD_PRIORITY_BLINKY_2	7

void blink_2(void);

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
/* my aux led */
static const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

K_THREAD_STACK_DEFINE(thread_stack_blinky_2, THREAD_STACK_SIZE_BLINKY_2);
struct k_thread thread_data_blinky_2;

int main(void)
{
	int ret;

	/* add another thread */
	k_tid_t thread_blinky_2;
	thread_blinky_2 = k_thread_create(&thread_data_blinky_2, thread_stack_blinky_2, THREAD_STACK_SIZE_BLINKY_2,
									   (k_thread_entry_t)blink_2, NULL, NULL, NULL, THREAD_PRIORITY_BLINKY_2, 0, K_NO_WAIT);

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

void blink_2(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led_2)) {
        return;
    }

    ret = gpio_pin_configure_dt(&led_2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return;
    }

    while (1) {
        gpio_pin_toggle_dt(&led_2);
        k_msleep(BLINKY_2_SLEEP_TIME_MS);
    }
}