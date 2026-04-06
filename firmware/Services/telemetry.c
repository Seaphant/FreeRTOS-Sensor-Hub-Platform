/**
 * @file telemetry.c
 */
#include "telemetry.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "app_config.h"
#include "fault_mgr.h"
#include "log.h"

static QueueHandle_t s_q;

static void format_tlm_line(char *buf, size_t buf_sz, const sample_frame_t *f)
{
    if (!buf || buf_sz == 0U || !f) {
        return;
    }

    /* tmp_c_milli -> decimal string without pulling in snprintf float. */
    int32_t c = f->tmp_c_milli;
    int32_t ci = c / 1000;
    int32_t cf = c % 1000;
    if (cf < 0) {
        cf = -cf;
    }

    (void)snprintf(buf, buf_sz,
                   "TLM seq=%u t_ms=%lu imu_ax_mg=%d imu_ay_mg=%d imu_az_mg=%d "
                   "gx_mdps=%d gy_mdps=%d gz_mdps=%d tmp_c=%ld.%03ld "
                   "vbus_mv=%u i_ma=%d",
                   (unsigned)f->seq, (unsigned long)f->t_ms, (int)f->imu_ax_mg,
                   (int)f->imu_ay_mg, (int)f->imu_az_mg, (int)f->gx_mdps, (int)f->gy_mdps,
                   (int)f->gz_mdps, (long)ci, (long)cf, (unsigned)f->vbus_mv, (int)f->i_ma);
}

void telemetry_init(void)
{
    if (s_q != NULL) {
        return;
    }
    s_q = xQueueCreate(TELEMETRY_QUEUE_DEPTH, sizeof(sample_frame_t));
}

status_t telemetry_publish(const sample_frame_t *frame)
{
    if (!frame || s_q == NULL) {
        return ST_ERR;
    }

    if (xQueueSend(s_q, frame, 0) != pdTRUE) {
        fault_raise(FAULT_TLM_QUEUE_DROP, FAULT_SEV_WARN, "q_full");
        return ST_BUSY;
    }
    return ST_OK;
}

void telemetry_task_entry(void *argument)
{
    (void)argument;
    sample_frame_t frame;
    char line[192];

    for (;;) {
        if (xQueueReceive(s_q, &frame, portMAX_DELAY) == pdTRUE) {
            format_tlm_line(line, sizeof(line), &frame);
            LOG_LINE("%s", line);
        }
    }
}
