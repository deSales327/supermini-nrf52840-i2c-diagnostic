/*
 * Standalone SuperMini nRF52840 I2C diagnostic.
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define FIRST_ADDR 0x08
#define LAST_ADDR  0x77

static const struct device *const i2c_dev = DEVICE_DT_GET(I2C_NODE);

static void scan_bus(void)
{
    int found = 0;

    printk("
I2C scan: SDA=P0.17 SCL=P0.20
");
    for (uint16_t address = FIRST_ADDR; address <= LAST_ADDR; address++) {
        int ret = i2c_write(i2c_dev, NULL, 0, address);
        if (ret == 0) {
            printk("FOUND 0x%02x
", address);
            found++;
        }
    }

    if (found == 0) {
        printk("No devices acknowledged.
");
    } else {
        printk("Found %d device%s.
", found, found == 1 ? "" : "s");
    }
}

int main(void)
{
    k_msleep(3000);
    printk("
========================================
");
    printk("SuperMini standalone I2C diagnostic
");
    printk("I2C0: SDA=P0.17, SCL=P0.20
");
    printk("========================================
");

    if (!device_is_ready(i2c_dev)) {
        printk("ERROR: i2c0 is not ready.
");
        return 0;
    }

    printk("USB CDC is running; i2c0 is ready.
");
    while (true) {
        scan_bus();
        k_msleep(5000);
    }
}
