/**
 * @file app.h
 * @brief Application orchestration for FreeRTOS bring-up (v0.1).
 */
#ifndef APP_H
#define APP_H

#include "app_types.h"

/**
 * Call from `main.c` after Cube `MX_*_Init()` and before the RTOS kernel starts.
 * Prints BOOT line, initializes services/drivers that do not need the scheduler.
 */
void app_init_early(void);

/**
 * Call from `MX_FREERTOS_Init()` user section (before `osKernelStart()`).
 * Creates application tasks and leaves default Cube tasks untouched.
 */
void app_rtos_create_tasks(void);

void app_set_mode(app_state_t mode);
void app_get_status(app_status_t *out);

#endif /* APP_H */
