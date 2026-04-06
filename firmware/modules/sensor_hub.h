/**
 * @file sensor_hub.h
 * @brief Aggregates sensor reads for one sample_frame_t (v0.1 stub until drivers land).
 */
#ifndef SENSOR_HUB_H
#define SENSOR_HUB_H

#include "app_types.h"
#include "status.h"

status_t sensor_hub_init(void);
status_t sensor_hub_read_all(sample_frame_t *out);

#endif /* SENSOR_HUB_H */
