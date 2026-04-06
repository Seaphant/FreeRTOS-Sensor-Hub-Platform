# Architecture — v0.1

## Layout rule

**STM32CubeIDE** generates **`Core/`** and HAL/CMSIS under **`Drivers/`** inside **`firmware/`**. Handwritten sensor aggregation and chip glue live in **`firmware/modules/`** (e.g. **`sensor_hub`**) so the name **`Drivers/`** is reserved for Cube HAL only.

```text
firmware/
├── App/           Application state, tasks, BOOT, LED
├── Services/      cli, telemetry, fault_mgr
├── modules/       sensor_hub (stub → real I2C later)
├── Common/        log, status
├── Core/          (Cube, later)
├── Drivers/       (Cube HAL only, later)
└── Middlewares/   (FreeRTOS, later)
```

## Layering

| Layer | Path | Rules |
|-------|------|-------|
| Cube / HAL | `Core/`, `Drivers/` (generated) | No product policy; regenerate from `.ioc` when possible |
| modules | `modules/` | No `vTaskDelay` / `xQueueSend` inside low-level chip helpers |
| Services | `Services/` | Telemetry queue, CLI, fault records |
| App | `App/` | Modes, task creation, sampling orchestration |

Chip-level helpers return **`status_t`**. Retries and **`fault_raise`** belong in tasks or thin glue, not in reusable chip read/write primitives.

## Modules (v0.1)

| Unit | Responsibility |
|------|----------------|
| `app.c` | `app_init_early()`, `app_rtos_create_tasks()`, **RUN**/**IDLE**, LD2, calls **`sensor_hub_read_all`** + **`telemetry_publish`** in **RUN** |
| `telemetry.c` | Queue owner; formats **`TLM`**; **`telemetry_task_entry`** |
| `cli.c` | **`HAL_UART_Receive`** on **`hlpuart1`**; line parser |
| `fault_mgr.c` | Ring of **`fault_record_t`** |
| `sensor_hub.c` | Builds **`sample_frame_t`** (**stub** in v0.1) |

## Data flow (v0.1)

```text
cli task ──► app_set_mode / app_get_status

app task ──► sensor_hub_read_all ──► telemetry_publish ──► queue ──► tlm task ──► UART (printf / LOG_LINE)
```

## FreeRTOS

- **No** UART DMA in v0.1.
- **IWDG** policy deferred past v0.1; enable only when service strategy is defined.

## Boot

`app_init_early()` prints **`BOOT`** using **`RCC->CSR`**, then **`__HAL_RCC_CLEAR_RESET_FLAGS()`**.

## Build

**STM32CubeIDE** with generated sources committed under **`firmware/`** is the supported workflow for v0.1.
