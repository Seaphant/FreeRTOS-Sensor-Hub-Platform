# STM32CubeIDE integration

Target: **NUCLEO-G474RE**, **STM32G474RET6**, **FreeRTOS**, **LPUART1** @ 115200 8N1, **I2C1** on **PB8**/**PB9**, **PE8** as **LD2**.

## Call order

### `app_init_early()`

- **File:** Cube-generated **`Core/Src/main.c`**
- **Place:** In a **`USER CODE`** section **after** all **`MX_*_Init()`** calls and **before** **`MX_FREERTOS_Init()`** (or before the kernel starts if you initialize FreeRTOS differently).
- **Purpose:** `fault_mgr`, telemetry queue, CLI, **`sensor_hub`** init; print **`BOOT`**.

### `app_rtos_create_tasks()`

- **File:** Cube-generated **`Core/Src/freertos.c`**, inside **`MX_FREERTOS_Init()`**
- **Place:** **`USER CODE`** region **after** Cube creates the CMSIS-RTOS default objects (if any) and **before** `osKernelStart()` runs.
- **Purpose:** Create **`app`**, **`tlm`**, and **`cli`** tasks (`xTaskCreate`).

Do **not** start these application tasks from **`main()`** after the scheduler is already running; create them from **`MX_FREERTOS_Init()`** so FreeRTOS objects exist before **`osKernelStart()`**.

## Safe edits vs regeneration

| Location | Policy |
|----------|--------|
| **`USER CODE n`…`USER CODE END n`** blocks in `main.c`, `freertos.c`, `stm32g4xx_hal_msp.c`, `stm32g4xx_it.c` | Preferred place for small hooks (`#include`, single calls). Cube preserves these across regenerate. |
| New **authored** `.c` / `.h` under **`App/`**, **`Services/`**, **`modules/`**, **`Common/`** | Never touched by Cube; add them to the IDE project manually. |
| **`Core/Src`**, **`Core/Inc`**, HAL under **`Drivers/`**, **`Middlewares/`** outside `USER CODE` | Treat as generated; change via **CubeMX** and **regenerate**, not hand-edited. |
| **`.ioc`** | Source of truth for clocks, pins, peripherals, middleware. |

If you must change IRQ priorities or alternate functions, prefer **CubeMX** → regenerate, or use **`USER CODE`** in **`stm32g4xx_hal_msp.c`** / **`stm32g4xx_it.c`** and re-apply after regenerate if Cube overwrites non-user regions.

## Keeping regeneration safe

1. Configure peripherals in **`.ioc`**; generate code.
2. Limit edits in generated files to **`USER CODE`** blocks.
3. Keep application logic in **`App/`**, **`Services/`**, **`modules/`**, **`Common/`**.
4. Commit **`.ioc`** and generated **`Core/`**, **`Drivers/`**, **`Middlewares/`** together so the tree always matches the project file.

## `printf` → **LPUART1** (high level)

1. Ensure **`LPUART1`** is initialized (**`MX_LPUART1_UART_Init()`**) before any **`printf`**.
2. Implement **`_write(int fd, char *ptr, int len)`** (or the syscall your toolchain uses) to send bytes with **`HAL_UART_Transmit(&hlpuart1, …)`** (blocking is acceptable for v0.1).
3. Optionally wrap **`LOG_LINE`** in **`log.h`** so all UART text goes through one path.

## Expected Cube symbols

| Symbol | Type | Role |
|--------|------|------|
| **`hlpuart1`** | `UART_HandleTypeDef` | ST-Link VCP, **CLI** + **`printf`** |
| **`hi2c1`** | `I2C_HandleTypeDef` | **I2C1** to sensors (init in v0.1; full use post–v0.1) |

Authored files declare them with **`extern`** where needed. If your `.ioc` uses different instance names, rename the **`extern`** to match.

## `Drivers/` vs authored sensor code

Cube places STM32 HAL/CMSIS under **`firmware/Drivers/`**. Keep **all** handwritten sensor aggregation and device glue in **`firmware/modules/`** so there is **one** meaning for **`Drivers/`** (HAL only) and no merge conflicts when updating Cube packs.

## LD2 pin

Default **LD2** mapping is **PE8**. If your board or Cube label differs, align **`APP_LED_*`** in **`app.c`** with the Cube pin definition.
