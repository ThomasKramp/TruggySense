# TESEO-LIV3F GPS on Teensy 4.1 — PlatformIO

## Wiring

### UART (recommended — simpler, no dummy-byte filtering)
| TESEO-LIV3F pin | Teensy 4.1 pin |
|-----------------|----------------|
| TX              | RX1 (pin 0)    |
| RX              | TX1 (pin 1)    |
| VCC             | 3.3V           |
| GND             | GND            |
| NRESET (opt.)   | Pin 2          |

### I2C
| TESEO-LIV3F pin | Teensy 4.1 pin |
|-----------------|----------------|
| SDA             | SDA (pin 18)   |
| SCL             | SCL (pin 19)   |
| VCC             | 3.3V           |
| GND             | GND            |
| NRESET (opt.)   | Pin 2          |

> Note: TESEO-LIV3F is a 3.3V device. Teensy 4.1 GPIO is also 3.3V — no level shifting needed.
> Add 4.7kΩ pull-ups on SDA/SCL to 3.3V for I2C.

## Building
Select your interface in `platformio.ini` by uncommenting the right build flag:
- `-DUSE_UART` for UART mode
- `-DUSE_I2C` for I2C mode
