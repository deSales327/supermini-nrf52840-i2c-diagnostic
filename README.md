# SuperMini nRF52840 I²C Diagnostic

Standalone Zephyr firmware for the eyelash_nano / SuperMini nRF52840 used in the Corne. It deliberately does not start ZMK, a key matrix, or the OLED driver. It brings up USB CDC ACM first, then scans I²C and prints every responding 7-bit address.

## Test configuration

- Board/flash layout: eyelash_nano-compatible UF2 bootloader
- I²C controller: i2c0
- SDA: P0.17
- SCL: P0.20
- Scan range: 0x08–0x77
- Output: USB CDC serial, scan repeated every five seconds

## Use the ready-made firmware

1. Open the latest successful run in the repository's Actions tab.
2. Download the artifact named supermini-i2c-diagnostic and extract it.
3. Connect the SuperMini by USB.
4. Enter DFU/bootloader mode, normally by double-tapping RESET.
5. Copy the file supermini-i2c-diagnostic.uf2 to the UF2 drive.
6. Wait for the board to reboot, then reconnect/select the new USB serial device.

USB CDC does not depend on the selected baud rate. A terminal at 115200 8N1 is fine.

## Read the result

Expected startup text is similar to:

    SuperMini standalone I2C diagnostic
    I2C0: SDA=P0.17, SCL=P0.20
    USB CDC is running; i2c0 is ready.

Then the scan reports FOUND 0x3c or FOUND 0x3d for a typical SSD1306 OLED, or:

    No devices acknowledged.

A multimeter reading about 3.3 V on SDA and SCL only proves that the lines are pulled high; it does not prove that I²C traffic is working.

## Build locally

The GitHub Actions workflow builds a Zephyr-only application and uses a local clean board definition. With west and the Zephyr build prerequisites available:

    west init -l config
    west update
    west zephyr-export
    west build -s bare -d build -b eyelash_nano -- -DBOARD_ROOT=$PWD/bare -DCONF_FILE=$PWD/bare/prj.conf

The output files are in build/zephyr/; the UF2 file is build/zephyr/zephyr.uf2.

## Interpret the result

- USB serial appears and i2c0 is ready: the standalone firmware is running and the I²C peripheral initialized.
- FOUND 0x3c or 0x3d: the OLED answered on P0.17/P0.20.
- No devices acknowledged: the peripheral started, but no device acknowledged; check power, GND, continuity, address, and pull-ups.
- USB serial appears but i2c0 is not ready: the board/pin configuration or I²C initialization needs investigation.
- No USB serial device after this standalone firmware: the failure is below the ZMK/OLED application layer; check the exact SuperMini hardware, its USB connection, and the runtime USB/MCU.

## License

MIT
