/**
 * @file log.h
 * @brief Thin logging wrapper (routes to printf -> LPUART1 once syscalls wired in Cube).
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/**
 * One complete line (adds CRLF for serial terminals).
 * Requires Cube retarget of printf/_write to LPUART1 (or ITM).
 */
#define LOG_LINE(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__)

#endif /* LOG_H */
