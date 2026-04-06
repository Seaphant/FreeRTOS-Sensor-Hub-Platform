/**
 * @file log.h
 * @brief Thread-safe line logging (printf -> LPUART1 via syscalls _write).
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/** Call once from app_init_early() after fault_mgr_init(), before other tasks run. */
void log_init(void);

/** Formats one line and emits CRLF. Safe from multiple tasks after log_init(). */
void log_line(const char *fmt, ...);

#define LOG_LINE(fmt, ...) log_line((fmt), ##__VA_ARGS__)

#endif /* LOG_H */
