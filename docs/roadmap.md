# Roadmap

## v0.1

- Documentation set in **`docs/`**, **`hardware/`**, root **`README.md`**.  
- Hand-written **`firmware/App`**, **`Services`**, **`modules`**, **`Common`**.  
- **STM32CubeIDE** project under **`firmware/`** (`.ioc`, **`Core/`**, HAL, **FreeRTOS**).  
- **`BOOT`**, **LD2** heartbeat, **CLI** (`help`, `status`, `start`, `stop`), **stub `TLM`** ~**10 Hz** in **`RUN`**.

**Out of scope:** DMA UART, binary frames, CMake, on-wire **`FLT`**, IWDG tuning, flash calibration.

## v1.0

- LSM6DSO / TMP117 / INA219 drivers (**I2C1**, timeouts, WHO_AM_I).  
- **`FLT`** UART lines + debounced bus faults.  
- **IWDG** with one refresh path.  
- Flash **`cal_blob_t`** + extended CLI.

## v1.1+

- DMA UART RX ring buffer  
- IMU DRDY / FIFO burst reads  

## Non-goals

Cloud, phone apps, OTA, custom PCB requirement, attitude estimation / fusion in this repo.
