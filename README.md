# SuperMini nRF52840 I²C Diagnostic

Minimal Zephyr firmware for the `eyelash_nano` board. It exercises the I²C controller used by the Corne OLED and prints every responding 7-bit address over the board's USB CDC ACM serial console.

## What it tests

- Board target: `eyelash_nano`
- I²C controller: `i2c0`
- SDA: `P0.17`
- SCL: `P0.20`
- Scan range: `0x08` through `0x77`
- Output: USB serial console, repeated every five seconds

The firmware is intentionally not a keyboard firmware. It does not initialize a key matrix, Bluetooth profile, OLED driver, or ZMK services. That keeps the test focused on the MCU, the two GPIOs, the I²C peripheral, and the USB serial path.

## Build locally

Install the Zephyr prerequisites and `west`, then run from this repository:

```sh
west init -l .
west update
west zephyr-export
west build -b eyelash_nano -d build .
```

The first `west update` downloads Zephyr and the `zmk-board-eyelash` board definition. Later builds can use:

```sh
west build -b eyelash_nano -d build .
```

The GitHub Actions workflow performs the same build automatically and publishes the generated firmware files as an artifact.

## Flash

1. Connect the SuperMini by USB.
2. Put it into its bootloader mode, normally by double-tapping RESET.
3. Copy `build/zephyr/zephyr.uf2` to the bootloader drive.
4. Wait for the board to reboot.

If the board does not expose a UF2 drive, use the generated `zephyr.hex` or `zephyr.bin` with the programmer appropriate for your bootloader.

## Read the scan result

After flashing, reconnect the board and open the USB serial device. A terminal configured as `115200 8N1` is suitable; USB CDC ACM does not depend on the baud rate.

Example output:

```text
SuperMini nRF52840 I2C diagnostic
Target: eyelash_nano / I2C0
Expected pins: SDA=P0.17, SCL=P0.20
i2c0 is ready.

I2C scan: bus=i2c0 SDA=P0.17 SCL=P0.20
Checking 7-bit addresses 0x08..0x77 ...
FOUND 0x3c
Found 1 device.
```

For a typical SSD1306 OLED, `0x3c` or `0x3d` is expected. A different I²C peripheral will report its own address. If the scan reports no devices, verify power, GND, continuity from the OLED to the SuperMini, and the pull-ups. A multimeter showing about 3.3 V on SDA/SCL only proves that the lines are pulled high; it does not prove that I²C traffic is working.

## Interpreting the result

- `i2c0 is ready` plus one or more `FOUND` lines: the MCU I²C peripheral and the P0.17/P0.20 path are communicating with a device.
- `i2c0 is ready` plus `No devices acknowledged`: the peripheral starts, but no connected device acknowledged. Check wiring, address, power, and pull-ups.
- `i2c0 is not ready`: the board definition, device tree, or I²C peripheral initialization did not complete.
- No USB serial output: check the USB cable, boot mode, and the serial device selected after the firmware reboots.

This scan cannot prove that every signal is electrically perfect under all conditions, but it is a much stronger test than measuring DC voltage alone.

## License

MIT
