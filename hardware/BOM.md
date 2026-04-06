# Bill of materials — lab build

| Qty | Item | Notes |
|-----|------|-------|
| 1 | NUCLEO-G474RE | STM32G474RET6, ST-Link |
| 1 | LSM6DSO breakout | 3.3 V I2C |
| 1 | TMP117 breakout | 3.3 V I2C |
| 1 | INA219 breakout | 3.3 V I2C; default addr `0x40` |
| — | Jumper wire | Short I2C runs |
| 2 | 4.7 kΩ resistor | Pull-ups if needed |

## Tools

USB data cable, multimeter, optional logic analyzer for I2C decode.

## v0.1

Firmware runs **without** sensors (stub **`TLM`**). Attach hardware before I2C driver milestones.
