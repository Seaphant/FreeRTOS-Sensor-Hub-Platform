# Pinout — NUCLEO-G474RE (v0.1)

## LPUART1 — ST-Link VCP (default)

Per ST **UM2505** §6.6.5, VCP uses **LPUART1** unless solder bridges move it to **USART1**.

| Function | MCU pin (typical Cube mapping) |
|----------|--------------------------------|
| LPUART1_TX | PA2 |
| LPUART1_RX | PA3 |

Confirm in generated **`main.h`** / **`.ioc`**.

## I2C1 — sensor bus

| Function | MCU pin |
|----------|---------|
| I2C1_SCL | PB8 |
| I2C1_SDA | PB9 |

Use **100 kHz** initially. Add **4.7 kΩ** pull-ups to **3.3 V** if a breakout lacks them.

## LD2

Configure **PE8** as **GPIO_Output** (push-pull), user label **LD2**. Match **`APP_LED_*`** in `app.c` if the silk or Cube name differs.

## NVIC

Document final UART vs I2C IRQ priorities in a comment once drivers use interrupts; v0.1 may run UART RX via polling in **`cli_task`**.
