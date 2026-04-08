/**
 * @file log.c
 */
#include "log.h"

#include <stdarg.h>

#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t s_log_mu;

void log_init(void)
{
    if (s_log_mu == NULL) {
        s_log_mu = xSemaphoreCreateMutex();
    }
}

void log_line(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    if (s_log_mu != NULL) {
        (void)xSemaphoreTake(s_log_mu, portMAX_DELAY);
    }

    (void)vprintf(fmt, ap);
    va_end(ap);
    (void)printf("\r\n");

    if (s_log_mu != NULL) {
        (void)xSemaphoreGive(s_log_mu);
    }
}
