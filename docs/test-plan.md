# Test plan — v0.1

## Intent

Verify flash, **UART**, **FreeRTOS** tasks, and protocol strings on **NUCLEO-G474RE** (**STM32G474RET6**) before I2C sensor driver work.

## Environment

- USB to ST-Link; serial terminal **115200 8N1**, local echo off
- Firmware built in **STM32CubeIDE** after **`firmware/`** integration per [cube-integration.md](cube-integration.md)

## Manual checks

| ID | Step | Pass criterion |
|----|------|----------------|
| T1 | Flash **Debug** build | No hard fault; **LD2** toggles |
| T2 | Reset; watch UART | Exactly one **`BOOT`** line |
| T3 | Send `help` | `OK commands: help \| status \| start \| stop` |
| T4 | Send `status` | Line starts with `OK`; `mode=IDLE` |
| T5 | Send `start` | `OK mode=RUN`; ~10 **`TLM`** lines per second |
| T6 | Send `stop` | `OK mode=IDLE`; **`TLM`** stops |
| T7 | Send unknown command | `ERR unknown_cmd` |
| T8 | Send line longer than CLI buffer | Line discarded safely; no lock-up |

## Preconditions

- Cube provides **`UART_HandleTypeDef hlpuart1`** and **`I2C_HandleTypeDef hi2c1`**
- **`app_init_early()`** and **`app_rtos_create_tasks()`** wired per [cube-integration.md](cube-integration.md)

## Post–v0.1

WHO_AM_I per sensor, I2C fault injection, IWDG reset reason, soak logs.

## Records

Keep UART captures under **`logs/`** (gitignored). Summarize results in release notes (e.g. tag `v0.1.0`).
