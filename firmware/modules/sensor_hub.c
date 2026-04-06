/**
 * @file sensor_hub.c
 */
#include "sensor_hub.h"

#include <string.h>

#include "main.h"

/* Cube-generated handle — confirm name matches .ioc (I2C1). */
extern I2C_HandleTypeDef hi2c1;

static uint16_t s_seq;

status_t sensor_hub_init(void)
{
    s_seq = 0U;
    /* TODO: probe LSM6DSO / TMP117 / INA219 (WHO_AM_I) and record faults. */
    (void)hi2c1;
    return ST_OK;
}

status_t sensor_hub_read_all(sample_frame_t *out)
{
    if (!out) {
        return ST_ERR;
    }

    (void)memset(out, 0, sizeof(*out));
    out->seq = s_seq++;
    out->t_ms = HAL_GetTick();

    /*
     * v0.1: publish plausible static stub values so telemetry/CLI can be exercised
     * without sensor drivers. Replace with real conversions in Milestone 2.
     */
    out->imu_ax_mg = 0;
    out->imu_ay_mg = 0;
    out->imu_az_mg = 1000; /* ~1 g on Z if board flat — illustrative only */
    out->gx_mdps = 0;
    out->gy_mdps = 0;
    out->gz_mdps = 0;
    out->tmp_c_milli = 23681; /* 23.681 C */
    out->vbus_mv = 3300U;
    out->i_ma = 0;

    /* TODO: return ST_UNIMPLEMENTED or propagate I2C errors once drivers exist. */
    return ST_OK;
}
