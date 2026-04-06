/**
 * @file app.c
 */
#include "app.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_config.h"
#include "cli.h"
#include "fault_mgr.h"
#include "log.h"
#include "sensor_hub.h"
#include "telemetry.h"

#include "main.h"
#include "stm32g4xx_hal.h"

#ifndef APP_LED_GPIO_Port
/** NUCLEO-G474RE: LD2 is typically PE8 — set in Cube or override here. */
#define APP_LED_GPIO_Port GPIOE
#define APP_LED_Pin GPIO_PIN_8
#endif

static app_state_t s_mode;
static uint16_t s_imu_hz;
static uint16_t s_slow_hz;
static bool s_cal_valid;
static uint32_t s_last_sample_ms;

static TaskHandle_t s_app_task;
static TaskHandle_t s_tlm_task;
static TaskHandle_t s_cli_task;

static void boot_print_reason(void)
{
    uint32_t csr = RCC->CSR;
    const char *reason = "OTHER";

    if ((csr & RCC_CSR_IWDGRSTF) != 0U) {
        reason = "IWDG";
    } else if ((csr & RCC_CSR_WWDGRSTF) != 0U) {
        reason = "WWDG";
    } else if ((csr & RCC_CSR_SFTRSTF) != 0U) {
        reason = "SFTR";
    } else if ((csr & RCC_CSR_PORRSTF) != 0U) {
        reason = "POR";
    } else if ((csr & RCC_CSR_PINRSTF) != 0U) {
        reason = "PIN";
    }

    LOG_LINE("BOOT mcu=STM32G474 t_ms=%lu reason=%s fw=%s cal=%u", (unsigned long)HAL_GetTick(),
             reason, APP_FW_VERSION_STR, s_cal_valid ? 1U : 0U);

    /* Clear reset flags for next boot interpretation */
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

void app_init_early(void)
{
    s_mode = APP_STATE_IDLE;
    s_imu_hz = APP_DEFAULT_IMU_HZ;
    s_slow_hz = APP_DEFAULT_SLOW_HZ;
    s_cal_valid = false;
    s_last_sample_ms = 0U;

    fault_mgr_init();
    telemetry_init();
    cli_init();
    (void)sensor_hub_init();

    boot_print_reason();
}

void app_set_mode(app_state_t mode)
{
    s_mode = mode;
}

void app_get_status(app_status_t *out)
{
    if (!out) {
        return;
    }

    taskENTER_CRITICAL();
    out->mode = s_mode;
    out->imu_hz = s_imu_hz;
    out->slow_hz = s_slow_hz;
    out->cal_valid = s_cal_valid;
    out->last_sample_ms = s_last_sample_ms;
    taskEXIT_CRITICAL();

    out->uptime_ms = (uint32_t)((uint64_t)xTaskGetTickCount() * (uint64_t)portTICK_PERIOD_MS);
}

static void led_heartbeat_step(void)
{
    HAL_GPIO_TogglePin(APP_LED_GPIO_Port, APP_LED_Pin);
}

static void app_task_entry_impl(void *arg)
{
    (void)arg;
    TickType_t last = xTaskGetTickCount();
    const TickType_t sample_period = pdMS_TO_TICKS(APP_SAMPLE_PERIOD_MS);
    uint32_t div = 0U;

    for (;;) {
        if (s_mode == APP_STATE_RUN) {
            sample_frame_t frame;
            (void)memset(&frame, 0, sizeof(frame));
            if (sensor_hub_read_all(&frame) == ST_OK) {
                frame.t_ms = HAL_GetTick();
                (void)telemetry_publish(&frame);
                taskENTER_CRITICAL();
                s_last_sample_ms = frame.t_ms;
                taskEXIT_CRITICAL();
            }
        }

        div++;
        if (div >= APP_HEARTBEAT_DIV) {
            div = 0U;
            led_heartbeat_step();
        }

        vTaskDelayUntil(&last, sample_period);
    }
}

void app_rtos_create_tasks(void)
{
    (void)xTaskCreate(app_task_entry_impl, "app", APP_TASK_STACK_WORDS, NULL, APP_TASK_PRIORITY,
                      &s_app_task);
    (void)xTaskCreate(telemetry_task_entry, "tlm", TELEMETRY_TASK_STACK_WORDS, NULL,
                      TELEMETRY_TASK_PRIORITY, &s_tlm_task);
    (void)xTaskCreate(cli_task_entry, "cli", CLI_TASK_STACK_WORDS, NULL, CLI_TASK_PRIORITY,
                      &s_cli_task);

    (void)s_app_task;
    (void)s_tlm_task;
    (void)s_cli_task;
}
