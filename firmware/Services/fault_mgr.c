/**
 * @file fault_mgr.c
 */
#include "fault_mgr.h"

#include <string.h>

#include "main.h"

#ifndef FAULT_LOG_CAPACITY
#define FAULT_LOG_CAPACITY 16U
#endif

static fault_record_t s_log[FAULT_LOG_CAPACITY];
static uint32_t s_head;
static uint32_t s_count;
static uint32_t s_active;

static void copy_detail(char *dst, size_t dst_sz, const char *src)
{
    if (!dst || dst_sz == 0U) {
        return;
    }
    dst[0] = '\0';
    if (!src) {
        return;
    }
    (void)strncpy(dst, src, dst_sz - 1U);
    dst[dst_sz - 1U] = '\0';
}

void fault_mgr_init(void)
{
    s_head = 0U;
    s_count = 0U;
    s_active = 0U;
    (void)memset(s_log, 0, sizeof(s_log));
}

void fault_raise(fault_code_t code, fault_severity_t sev, const char *detail)
{
    fault_record_t *slot = &s_log[s_head];
    slot->t_ms = HAL_GetTick();
    slot->code = code;
    slot->severity = sev;
    copy_detail(slot->detail, sizeof(slot->detail), detail);

    s_head = (s_head + 1U) % FAULT_LOG_CAPACITY;
    if (s_count < FAULT_LOG_CAPACITY) {
        s_count++;
    }
    if (sev >= FAULT_SEV_WARN) {
        s_active++;
    }
}

void fault_clear_all(void)
{
    fault_mgr_init();
}

uint32_t fault_active_count(void)
{
    return s_active;
}

uint32_t fault_snapshot(fault_record_t *out, uint32_t max_out)
{
    if (!out || max_out == 0U) {
        return 0U;
    }

    uint32_t n = s_count < max_out ? s_count : max_out;
    if (n == 0U) {
        return 0U;
    }

    /* Chronological order: if not full, records occupy [0 .. count-1]. If full, oldest is at head. */
    uint32_t start = (s_count < FAULT_LOG_CAPACITY) ? 0U : s_head;

    for (uint32_t i = 0U; i < n; i++) {
        uint32_t idx = (start + i) % FAULT_LOG_CAPACITY;
        out[i] = s_log[idx];
    }
    return n;
}
