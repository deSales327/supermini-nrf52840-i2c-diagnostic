/*
 * Standalone SuperMini nRF52840 I2C diagnostic.
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/usb/usb_device.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define CDC_NODE DT_NODELABEL(cdc_acm_uart)
#define FIRST_ADDR 0x08
#define LAST_ADDR  0x77

static const struct device *const i2c_dev = DEVICE_DT_GET(I2C_NODE);
static const struct device *const cdc_dev = DEVICE_DT_GET(CDC_NODE);

static void scan_bus(void)
{
    int found = 0;
    const uint8_t probe = 0x00;

    printk("\r\nI2C scan: SDA=P0.17 SCL=P0.20\r\n");
    for (uint16_t address = FIRST_ADDR; address <= LAST_ADDR; address++) {
        /*
         * The Nordic TWI driver is not probed with a zero-length transfer.
         * 0x00 is a valid SSD1306 command-control byte and is harmless when
         * sent without a following command byte.
         */
        int ret = i2c_write(i2c_dev, &probe, sizeof(probe), address);
        if (ret == 0) {
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

int main(void)
{
    uint32_t dtr = 0U;
    int ret;

    if (!device_is_ready(cdc_dev)) {
        return 0;
    }

    /* Zephyr 3.5's CDC ACM sample enables the legacy USB stack explicitly. */
    ret = usb_enable(NULL);
    if (ret != 0) {
        return 0;
    }

    /*
     * Keep the board idle and enumerated until a serial terminal opens it.
     * No I2C access can occur before DTR is asserted by the host.
     */
    while (true) {
        ret = uart_line_ctrl_get(cdc_dev, UART_LINE_CTRL_DTR, &dtr);
        if (ret == 0 && dtr != 0U) {
            break;
        }
        k_msleep(100);
    }

    k_msleep(100);
    printk("\r\n========================================\r\n");
    printk("SuperMini standalone I2C diagnostic\r\n");
    printk("I2C0: SDA=P0.17, SCL=P0.20\r\n");
    printk("========================================\r\n");

    if (!device_is_ready(i2c_dev)) {
        printk("ERROR: i2c0 is not ready.\r\n");
        return 0;
    }

    printk("USB CDC is running; i2c0 is ready.\r\n");
    while (true) {
        scan_bus();
        k_msleep(5000);
    }
}
