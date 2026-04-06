/**
 * @file status.h
 * @brief Small shared status type for drivers and services (v0.1).
 */
#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

typedef enum {
    ST_OK = 0,
    ST_ERR = 1,
    ST_TIMEOUT = 2,
    ST_BUSY = 3,
    /** Driver path not yet implemented (stub return). */
    ST_UNIMPLEMENTED = 4
} status_t;

#endif /* STATUS_H */
