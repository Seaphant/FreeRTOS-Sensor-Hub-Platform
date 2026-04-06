# stm32-rtos-sensor-hub

Bare-metal **C** firmware for **STM32G474RET6** on **NUCLEO-G474RE** with **FreeRTOS**: **I2C1** (PB8/PB9) bring-up toward **LSM6DSO**, **TMP117**, and **INA219**; **LPUART1** (ST-Link VCP) for **CLI** and text **telemetry**; compact **fault** records in RAM.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## What this is

Solo embedded portfolio work: RTOS task split, UART protocol, fault bookkeeping, and clear boundaries between Cube HAL and application code—no cloud, GUI, or wireless stacks.

## v0.1 scope

- Hand-written tree under `firmware/{App,Services,modules,Common}` (see [firmware/README.md](firmware/README.md)).
- **STM32CubeIDE** output (`Core/`, HAL **`Drivers/`**, FreeRTOS, `.ioc`, linker script) is added later **under `firmware/`** (see [docs/cube-integration.md](docs/cube-integration.md)).
- **BOOT** line, **LD2** heartbeat, **CLI** (`help`, `status`, `start`, `stop`), **stub `TLM`** at **~10 Hz** in **RUN**.
- **No** UART DMA, **no** binary telemetry, **no** CMake build in v0.1.

## Hardware

| Item | Role |
|------|------|
| **NUCLEO-G474RE** | **STM32G474RET6**, ST-Link, **LPUART1** VCP |
| **LSM6DSO** | IMU (post–v0.1 driver work) |
| **TMP117** | Temperature |
| **INA219** | Bus voltage / current |

See [hardware/BOM.md](hardware/BOM.md), [hardware/wiring.md](hardware/wiring.md), [docs/pinout.md](docs/pinout.md).

## Architecture

- **App** — `app_init_early()`, `app_rtos_create_tasks()`, **RUN**/**IDLE**, LED, sampling cadence.
- **Services** — `telemetry` (queue + **`TLM`**), `cli` (line-oriented **LPUART1**), `fault_mgr` (ring buffer).
- **modules** — `sensor_hub` builds one **`sample_frame_t`** (**stub** through v0.1).
- **Common** — `status_t`, `log.h` (`printf`; retarget to **LPUART1** in Cube).

Diagram: [diagrams/architecture.txt](diagrams/architecture.txt). Details: [docs/architecture.md](docs/architecture.md).

## Repo layout

```text
docs/            Spec, protocol, tests, pinout, roadmap, Cube integration
hardware/        BOM, wiring
firmware/        Hand-written modules + (later) Cube output
diagrams/        ASCII architecture
scripts/         UART smoke (later)
tests/           Test notes (see tests/README.md)
```

## Build / flash (after Cube import)

1. Open the **STM32CubeIDE** project under **`firmware/`**.
2. Add include paths for **`App`**, **`Services`**, **`modules`**, **`Common`** ([firmware/README.md](firmware/README.md)).
3. In Cube: **FreeRTOS**, **LPUART1** @ 115200 8N1, **I2C1** on **PB8**/**PB9**, **PE8** as **LD2** output.
4. Wire `app_init_early()` and `app_rtos_create_tasks()` per [docs/cube-integration.md](docs/cube-integration.md).
5. Flash **NUCLEO-G474RE**; serial console **115200 8N1**.

## UART examples

**Boot**

```text
BOOT mcu=STM32G474 t_ms=0 reason=POR fw=0.1.0 cal=0
```

**CLI**

```text
help
OK commands: help | status | start | stop
start
OK mode=RUN
```

**Telemetry (stub, RUN)**

```text
TLM seq=0 t_ms=120 imu_ax_mg=0 imu_ay_mg=0 imu_az_mg=1000 gx_mdps=0 gy_mdps=0 gz_mdps=0 tmp_c=23.681 vbus_mv=3300 i_ma=0
```

## Docs index

| File | Purpose |
|------|---------|
| [docs/technical-spec.md](docs/technical-spec.md) | Engineering spec |
| [docs/architecture.md](docs/architecture.md) | Layers and tasks |
| [docs/protocol.md](docs/protocol.md) | `BOOT`, `TLM`, CLI |
| [docs/cube-integration.md](docs/cube-integration.md) | Cube hooks and safe edits |
| [docs/test-plan.md](docs/test-plan.md) | v0.1 checks |
| [docs/pinout.md](docs/pinout.md) | Pin assignment |
| [docs/roadmap.md](docs/roadmap.md) | Milestones |

## Roadmap

| Tag | Content |
|-----|---------|
| **v0.1** | Docs + skeleton C + Cube project boots: **BOOT**, LD2, CLI, stub **`TLM`** |
| **v1.0** | Real I2C sensors, **`FLT`** lines, IWDG policy, flash calibration |

## Author

**Seaphant**

## License

MIT — see [LICENSE](LICENSE).
