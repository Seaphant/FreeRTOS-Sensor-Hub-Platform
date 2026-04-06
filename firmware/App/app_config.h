/**
 * @file app_config.h
 * @brief Compile-time defaults for v0.1 bring-up.
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "FreeRTOS.h"

/** Firmware semantic version string (also print in BOOT line). */
#define APP_FW_VERSION_STR "0.1.0"

/** Default telemetry rates (Hz) — clamped further in v1 CLI. */
#define APP_DEFAULT_IMU_HZ 100U
#define APP_DEFAULT_SLOW_HZ 10U

/** Queue depth for sample_frame_t pending telemetry. */
#define TELEMETRY_QUEUE_DEPTH 16U

/** Task stack sizes (words, not bytes). Tune after measuring high-water marks. */
#define APP_TASK_STACK_WORDS 384U
#define TELEMETRY_TASK_STACK_WORDS 384U
#define CLI_TASK_STACK_WORDS 512U

/**
 * FreeRTOS priorities: higher number = higher priority.
 * Keep CLI below telemetry so bursts do not starve sampling in later milestones.
 */
#define APP_TASK_PRIORITY (tskIDLE_PRIORITY + 1U)
#define TELEMETRY_TASK_PRIORITY (tskIDLE_PRIORITY + 3U)
#define CLI_TASK_PRIORITY (tskIDLE_PRIORITY + 2U)

/** Sensor publish cadence while in RUN (v0.1 stub path). */
#define APP_SAMPLE_PERIOD_MS 100U

/** LED toggles every N sample periods (N * APP_SAMPLE_PERIOD_MS = heartbeat rate). */
#define APP_HEARTBEAT_DIV 5U

/** CLI input buffer size (bytes). */
#define CLI_LINE_MAX 96U

#endif /* APP_CONFIG_H */
