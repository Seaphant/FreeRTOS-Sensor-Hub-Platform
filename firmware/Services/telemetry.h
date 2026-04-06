/**
 * @file telemetry.h
 * @brief Formats and forwards sample_frame_t as TLM UART lines (v0.1 stub data OK).
 */
#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "app_types.h"
#include "status.h"

void telemetry_init(void);
status_t telemetry_publish(const sample_frame_t *frame);
void telemetry_task_entry(void *argument);

#endif /* TELEMETRY_H */
