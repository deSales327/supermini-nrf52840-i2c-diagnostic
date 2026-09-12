/*
 * SuperMini nRF52840 I2C diagnostic for ZMK
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define SCAN_FIRST_ADDRESS 0x08
#define SCAN_LAST_ADDRESS  0x77
#define SCAN_INTERVAL_MS   5000

static const struct device *const i2c_dev = DEVICE_DT_GET(I2C_NODE);

static int probe_address(uint16_t address)
{
    /* A zero-byte write is the usual I2C address probe. */
    return i2c_write(i2c_dev, NULL, 0, address);
}

static void scan_bus(void)
{
    int found = 0;
    printk("\r\nI2C scan: bus=i2c0 SDA=P0.17 SCL=P0.20\r\n");
    printk("Checking 7-bit addresses 0x%02x..0x%02x ...\r\n",
           SCAN_FIRST_ADDRESS, SCAN_LAST_ADDRESS);

    for (uint16_t address = SCAN_FIRST_ADDRESS;
         address <= SCAN_LAST_ADDRESS;
         address++) {
        if (probe_address(address) == 0) {
            printk("FOUND 0x%02x\r\n", address);
            found++;
        }
    }

    if (found == 0) {
        printk("No devices acknowledged.\r\n");
    } else {
        printk("Found %d device%s.\r\n", found, found == 1 ? "" : "s");
    }
}

static void diagnostic_thread(void *unused_a, void *unused_b, void *unused_c)
{
    ARG_UNUSED(unused_a);
    ARG_UNUSED(unused_b);
    ARG_UNUSED(unused_c);

    printk("\r\n========================================\r\n");
    printk("SuperMini nRF52840 I2C diagnostic\r\n");
    printk("Target: eyelash_nano / I2C0\r\n");
    printk("Expected pins: SDA=P0.17, SCL=P0.20\r\n");
    printk("========================================\r\n");

    if (!device_is_ready(i2c_dev)) {
        printk("ERROR: i2c0 is not ready.\r\n");
        return;
    }

    printk("i2c0 is ready.\r\n");

    while (true) {
        scan_bus();
        k_msleep(SCAN_INTERVAL_MS);
    }
}

K_THREAD_DEFINE(supermini_i2c_diagnostic, 2048,
                diagnostic_thread, NULL, NULL, NULL, 5, 0, 0);

