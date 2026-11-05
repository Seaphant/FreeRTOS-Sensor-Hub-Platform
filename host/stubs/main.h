#ifndef HOST_STUB_MAIN_H
#define HOST_STUB_MAIN_H
#include <stdint.h>
extern uint32_t host_tick_ms;
#define HAL_GetTick() (host_tick_ms)
#endif
