/**
 * @file cli.h
 * @brief Line-oriented UART CLI (help/status/stop/start stubs for v0.1).
 */
#ifndef CLI_H
#define CLI_H

#include "status.h"

void cli_init(void);
void cli_task_entry(void *argument);

#endif /* CLI_H */
