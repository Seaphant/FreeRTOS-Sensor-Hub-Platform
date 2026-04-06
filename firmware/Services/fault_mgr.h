/**
 * @file fault_mgr.h
 * @brief RAM ring buffer for faults (v0.1). UART FLT lines are post–v0.1.
 *
 * All APIs are task-context only in v0.1 (not ISR-safe). Updates are protected
 * with FreeRTOS critical sections for multi-task use.
 */
#ifndef FAULT_MGR_H
#define FAULT_MGR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    FAULT_SEV_INFO = 0,
    FAULT_SEV_WARN,
    FAULT_SEV_ERROR,
    FAULT_SEV_CRITICAL
} fault_severity_t;

typedef enum {
    FAULT_OK = 0,
    FAULT_I2C_NACK,
    FAULT_I2C_TIMEOUT,
    FAULT_I2C_BUS,
    FAULT_WHOAMI_MISMATCH,
    FAULT_RANGE_HIGH,
    FAULT_RANGE_LOW,
    FAULT_TLM_QUEUE_DROP,
    FAULT_UART_OVERRUN,
    FAULT_CAL_CRC,
    FAULT_WDT_RESET_PREV,
    FAULT_UNIMPLEMENTED
} fault_code_t;

typedef struct {
    uint32_t t_ms;
    fault_code_t code;
    fault_severity_t severity;
    char detail[24];
} fault_record_t;

void fault_mgr_init(void);
void fault_raise(fault_code_t code, fault_severity_t sev, const char *detail);
void fault_clear_all(void);
uint32_t fault_active_count(void);
uint32_t fault_snapshot(fault_record_t *out, uint32_t max_out);

#endif /* FAULT_MGR_H */
