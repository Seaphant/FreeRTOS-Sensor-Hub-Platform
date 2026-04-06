# Wiring

## Power

- **3V3** and **GND** from Nucleo to each breakout.  
- **3.3 V** logic only on **I2C1** and GPIO.

## I2C1

- **PB8** → **SCL** on all devices.  
- **PB9** → **SDA** on all devices.  
- Verify **unique** 7-bit addresses (SA0/ADDR pins per board).

## UART

No extra wiring: ST-Link USB provides **LPUART1** VCP.

## Practice

- Keep I2C jumpers **short**.  
- Start **100 kHz**; increase only after stable reads.

## Order

1. Flash and verify **`BOOT`** + CLI **without** sensors.  
2. Connect I2C bus; confirm pull-ups and voltage.  
3. Proceed to WHO_AM_I (v1).
