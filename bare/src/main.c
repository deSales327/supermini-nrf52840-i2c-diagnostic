/*
 * Standalone Zephyr USB CDC diagnostic for the SuperMini nRF52840.
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    k_msleep(3000);

    printk("\r\n========================================\r\n");
    printk("SuperMini nRF52840 standalone Zephyr\r\n");
    printk("USB CDC ACM is running\r\n");
    printk("I2C is intentionally disabled for this test\r\n");
    printk("========================================\r\n");

    while (true) {
        printk("USB CDC alive\r\n");
        k_msleep(1000);
    }

    return 0;
}
