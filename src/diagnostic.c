/*
 * SuperMini nRF52840 USB diagnostic for ZMK
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define USB_STARTUP_DELAY_MS 3000
#define HEARTBEAT_INTERVAL_MS 1000

static void diagnostic_thread(void *unused_a, void *unused_b, void *unused_c)
{
    ARG_UNUSED(unused_a);
    ARG_UNUSED(unused_b);
    ARG_UNUSED(unused_c);

    k_msleep(USB_STARTUP_DELAY_MS);

    printk("\r\n========================================\r\n");
    printk("SuperMini nRF52840 USB diagnostic\r\n");
    printk("USB CDC started; I2C scan temporarily disabled\r\n");
    printk("========================================\r\n");

    while (true) {
        printk("USB CDC alive\r\n");
        k_msleep(HEARTBEAT_INTERVAL_MS);
    }
}

K_THREAD_DEFINE(supermini_usb_diagnostic, 1024,
                diagnostic_thread, NULL, NULL, NULL, 5, 0, 0);
