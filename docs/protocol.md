# UART protocol — v0.1

## Transport

- **Port:** ST-Link **VCP**
- **MCU:** **LPUART1** (NUCLEO-G474RE default)
- **Format:** 115200 **8N1**
- **Lines:** terminated by `\n` or `\r\n`

## `BOOT` (once per reset)

```text
BOOT mcu=STM32G474 t_ms=<u32> reason=<POR|PIN|IWDG|WWDG|SFTR|OTHER> fw=<semver> cal=<0|1>
```

Example:

```text
BOOT mcu=STM32G474 t_ms=0 reason=POR fw=0.1.0 cal=0
```

## `TLM`

Emitted in **RUN** when the application enqueues a frame (**stub** values in v0.1).

```text
TLM seq=<u16> t_ms=<u32> imu_ax_mg=<i16> imu_ay_mg=<i16> imu_az_mg=<i16> gx_mdps=<i16> gy_mdps=<i16> gz_mdps=<i16> tmp_c=<dec> vbus_mv=<u16> i_ma=<i16>
```

`tmp_c` is degrees Celsius in decimal (three fractional digits typical).

Example:

```text
TLM seq=7 t_ms=812 imu_ax_mg=0 imu_ay_mg=0 imu_az_mg=1000 gx_mdps=0 gy_mdps=0 gz_mdps=0 tmp_c=23.681 vbus_mv=3300 i_ma=0
```

## CLI — v0.1

| Input | Output |
|-------|--------|
| `help` | `OK commands: help \| status \| start \| stop` |
| `status` | `OK uptime_ms=<u32> mode=<IDLE\|RUN\|CAL> imu_hz=<u16> slow_hz=<u16> last_sample_ms=<u32> faults=<u32> cal=<0\|1>` |
| `start` | `OK mode=RUN` |
| `stop` | `OK mode=IDLE` |

Unknown verb:

```text
ERR unknown_cmd
```

## Post–v0.1

- UART lines prefixed **`FLT`**
- Additional CLI verbs (rates, calibration, fault dump)

## Versioning

Bump **`fw=`** in **`BOOT`** when **`TLM`** fields or CLI semantics change.
