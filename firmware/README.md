# Firmware

## Hand-written sources

Maintain product code under:

- **`App/`** — application and RTOS task creation
- **`Services/`** — **CLI**, **telemetry**, **fault_mgr**
- **`modules/`** — **`sensor_hub`** and future chip-level glue (**not** Cube HAL)
- **`Common/`** — **shared types**, **`log.h`**

**Recommendation:** Keep Cube’s HAL/CMSIS exclusively in the generated **`Drivers/`** folder. Put **`sensor_hub`** (and similar) in **`modules/`** so the directory name **`Drivers/`** never mixes ST HAL with authored files.

## Layout after Cube import

Place **STM32CubeIDE** / CubeMX output **under this directory** next to the folders above:

```text
firmware/
├── README.md                 (this file)
├── *.ioc
├── Core/                     Cube: startup, main.c, stm32g4xx_it.c, system_*.c, …
├── Drivers/                  Cube: CMSIS + STM32G4 HAL only
├── Middlewares/              Cube: FreeRTOS
├── STM32G474RETX_FLASH.ld    (or equivalent from Cube)
├── App/
├── Services/
├── modules/
└── Common/
```

## Include paths (STM32CubeIDE)

**Project → Properties → C/C++ Build → MCU GCC Compiler → Include paths** — add (adjust to your project root):

- `${ProjDirPath}/App`
- `${ProjDirPath}/Services`
- `${ProjDirPath}/modules`
- `${ProjDirPath}/Common`

## Integration hooks

See [docs/cube-integration.md](../docs/cube-integration.md) for **`main.c`**, **`freertos.c`**, and safe regeneration.

## Handles

Authored code expects:

- **`UART_HandleTypeDef hlpuart1`**
- **`I2C_HandleTypeDef hi2c1`**

If your `.ioc` renames handles, update the matching **`extern`** declarations in authored sources.

## printf

Retarget **`_write`** (or the syscalls implementation your newlib build uses) to **LPUART1** so **`LOG_LINE`** / **`printf`** reach the ST-Link VCP. Details: [docs/cube-integration.md](../docs/cube-integration.md).
