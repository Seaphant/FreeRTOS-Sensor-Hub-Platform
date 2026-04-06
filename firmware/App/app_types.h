/**
 * @file app_types.h
 * @brief Core application types shared across App/Services/Drivers.
 */
#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    APP_STATE_IDLE = 0,
    APP_STATE_RUN = 1,
    APP_STATE_CAL = 2
} app_state_t;

/**
 * One published telemetry frame (see docs/protocol.md).
 * Integer scales match README / protocol examples.
 */
typedef struct {
    uint16_t seq;
    uint32_t t_ms;
    int16_t imu_ax_mg;
    int16_t imu_ay_mg;
    int16_t imu_az_mg;
    int16_t gx_mdps;
    int16_t gy_mdps;
    int16_t gz_mdps;
    /** Degrees C * 1000 for v0.1 integer path (telemetry prints decimal). */
    int32_t tmp_c_milli;
    uint16_t vbus_mv;
    int16_t i_ma;
} sample_frame_t;

typedef struct {
    app_state_t mode;
    uint16_t imu_hz;
    uint16_t slow_hz;
    uint32_t uptime_ms;
    uint32_t last_sample_ms;
    bool cal_valid;
} app_status_t;

#endif /* APP_TYPES_H */
