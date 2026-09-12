# SuperMini nRF52840 I²C Diagnostic

Minimal ZMK-based diagnostic firmware for the eyelash_nano nRF52840 board. It scans the I²C bus used by the Corne OLED and reports every responding 7-bit address over the board's USB CDC ACM serial console.

## What it tests

- Board: eyelash_nano
- I²C controller: i2c0
- SDA: P0.17
- SCL: P0.20
- Scan range: 0x08 through 0x77
- Output: USB serial console, repeated every five seconds

The build uses ZMK because the eyelash_nano board definition is maintained as a ZMK board module. The diagnostic adds only one small thread; it does not configure a key matrix, OLED driver, or keymap.

## Build with GitHub Actions

Pushes to main build automatically through the official ZMK reusable workflow. Open the repository's Actions tab and download the artifact named supermini-i2c-diagnostic from a successful run.

## Build locally

Install the ZMK prerequisites and west, then run from this repository:

~~~
west init -l config
west update
west zephyr-export
west build -s zmk/app -d build -b eyelash_nano -- `
  -DZMK_CONFIG=$PWD/config `
  -DSHIELD=supermini_diag
~~~

On Windows PowerShell, use the absolute path to the config directory for ZMK_CONFIG if the shell does not expand the path as shown.

The first west update downloads ZMK and the zmk-board-eyelash board definition. Later builds can use the last west build command.

## Flash

1. Connect the SuperMini by USB.
2. Put it into bootloader mode, normally by double-tapping RESET.
3. Copy the generated build/zephyr/zmk.uf2 or the UF2 file from the Actions artifact to the bootloader drive.
4. Wait for the board to reboot.

If the board does not expose a UF2 drive, use the generated binary supported by its bootloader.

## Read the scan result

After flashing, reconnect the board and open its USB serial device. A terminal configured as 115200 8N1 is suitable; USB CDC ACM does not depend on the baud rate.

Example output:

~~~
SuperMini nRF52840 I2C diagnostic
Target: eyelash_nano / I2C0
Expected pins: SDA=P0.17, SCL=P0.20
i2c0 is ready.

I2C scan: bus=i2c0 SDA=P0.17 SCL=P0.20
Checking 7-bit addresses 0x08..0x77 ...
FOUND 0x3c
Found 1 device.
~~~

For a typical SSD1306 OLED, 0x3c or 0x3d is expected. If no device is found, verify power, GND, continuity from the OLED to the SuperMini, and the pull-ups. A multimeter showing about 3.3 V on SDA/SCL only proves that the lines are pulled high; it does not prove that I²C traffic is working.

## Interpreting the result

- i2c0 is ready plus one or more FOUND lines: the MCU I²C peripheral and the P0.17/P0.20 path are communicating with a device.
- i2c0 is ready plus No devices acknowledged: the peripheral starts, but no connected device acknowledged. Check wiring, address, power, and pull-ups.
- i2c0 is not ready: the board definition, device tree, or I²C peripheral initialization did not complete.
- No USB serial output: check the USB cable, boot mode, and the serial device selected after the firmware reboots.

This scan cannot prove that every signal is electrically perfect under all conditions, but it is a much stronger test than measuring DC voltage alone.

## License

MIT

