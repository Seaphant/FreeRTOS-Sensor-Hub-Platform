# Technical specification — v0.1

## Purpose

Firmware for **NUCLEO-G474RE** using **FreeRTOS** to decouple **UART I/O**, **telemetry formatting**, and a future **I2C1** sensor path. **v0.1** proves integration: **`BOOT`**, **CLI**, **stub `TLM`**, **LED heartbeat**, and module layout. **v1** replaces sensor stubs with real drivers and adds **IWDG** and **flash calibration**.

## Target

| Item | Value |
|------|--------|
| MCU | STM32G474RET6 |
| Board | ST NUCLEO-G474RE |
| Debugger | On-board ST-Link |
| Host link | USB VCP ↔ **LPUART1** (default Nucleo wiring, UM2505 §6.6.5) |

## Peripherals (v0.1)

| Peripheral | Configuration |
|------------|----------------|
| **LPUART1** | 115200 8N1, CLI + `printf` output |
| **I2C1** | PB8 SCL, PB9 SDA, **100 kHz** Standard Mode (init only required in v0.1) |
| **GPIO PE8** | **LD2** push-pull output |

**Deferred to v1:** IWDG enabled in field configuration, UART `FLT` lines, flash `cal_blob_t`.

## Sensors (v1 target; not required for v0.1 pass)

| Device | Bus | Typical 7-bit address |
|--------|-----|------------------------|
| LSM6DSO | I2C1 | `0x6A` / `0x6B` |
| TMP117 | I2C1 | `0x48`–`0x4B` |
| INA219 | I2C1 | `0x40` |

## RTOS

| Task | Priority (higher = more urgent) | Role |
|------|----------------------------------|------|
| `tlm` | `IDLE+3` | Wait on queue, print `TLM` |
| `cli` | `IDLE+2` | Read bytes, dispatch `help` / `status` / `start` / `stop` |
| `app` | `IDLE+1` | `RUN`/`IDLE` policy, **100 ms** cadence, LED toggle every **5** periods |

**IPC:** FreeRTOS queue of **`sample_frame_t`**, depth **16**. Overflow raises **`FAULT_TLM_QUEUE_DROP`** in **`fault_mgr`**.

## Telemetry

Single ASCII line per frame, prefix **`TLM`**. Fields per [protocol.md](protocol.md). **v0.1** uses **stub** numeric content from **`sensor_hub`** in **`firmware/modules/`**.

## CLI (v0.1)

`help`, `status`, `start`, `stop` only. Grammar: [protocol.md](protocol.md).

## Fault handling (v0.1)

**`fault_mgr`** stores records in RAM (fixed ring). **UART `FLT` lines** are **v1**. Queue drop and CLI line overflow increment internal faults.

## Sample timing

| Signal | v0.1 |
|--------|------|
| Stub sample publish | **10 Hz** in `RUN` (`app` task period **100 ms`) |
| LED toggle | Every **500 ms** (every **5** sample periods) |

## Acceptance

- `BOOT` after reset.  
- LD2 blinks.  
- CLI verbs work.  
- ~**10 Hz** `TLM` in `RUN`.

## Revision

| Rev | Note |
|-----|------|
| 0.1 | v0.1 documentation + skeleton firmware |
