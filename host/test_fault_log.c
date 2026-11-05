#include "fault_mgr.h"

#include <stdio.h>
#include <string.h>

uint32_t host_tick_ms;

static int failed;

static void check(const char *name, int passed)
{
    printf("%s %s\n", passed ? "PASS" : "FAIL", name);
    if (!passed)
        failed = 1;
}

int main(void)
{
    fault_record_t records[16];
    uint32_t count;
    uint32_t index;
    char long_detail[] = "this detail is longer than the slot";

    fault_mgr_init();
    check("empty log snapshots nothing", fault_snapshot(records, 16) == 0);
    check("null output snapshots nothing", fault_snapshot(0, 16) == 0);

    host_tick_ms = 10;
    fault_raise(FAULT_TLM_QUEUE_DROP, FAULT_SEV_INFO, "info");
    check("info does not count as a warn", fault_active_count() == 0);

    for (index = 0; index < 16; index++) {
        host_tick_ms = 1000U + index;
        fault_raise(FAULT_TLM_QUEUE_DROP, FAULT_SEV_WARN, "q_full");
    }
    count = fault_snapshot(records, 16);
    check("sixteen warns fill the log", count == 16);
    check("warn count is sixteen", fault_active_count() == 16);
    check("oldest kept is the first warn", records[0].t_ms == 1000U);
    check("newest is the sixteenth warn", records[15].t_ms == 1015U);

    host_tick_ms = 2000;
    fault_raise(FAULT_UART_OVERRUN, FAULT_SEV_WARN, "cli_line");
    count = fault_snapshot(records, 16);
    check("seventeenth record drops the oldest", count == 16);
    check("oldest after wrap is the second warn", records[0].t_ms == 1001U);
    check("newest after wrap is the CLI overrun",
          records[15].code == FAULT_UART_OVERRUN && records[15].t_ms == 2000U);
    check("warn count keeps the dropped record", fault_active_count() == 17);
    check("detail is the CLI tag", strcmp(records[15].detail, "cli_line") == 0);

    fault_clear_all();
    fault_raise(FAULT_UART_OVERRUN, FAULT_SEV_ERROR, long_detail);
    count = fault_snapshot(records, 16);
    check("detail is cut to 23 characters",
          count == 1 && strlen(records[0].detail) == 23);

    fault_raise(FAULT_TLM_QUEUE_DROP, FAULT_SEV_WARN, 0);
    count = fault_snapshot(records, 16);
    check("null detail is an empty string",
          count == 2 && records[1].detail[0] == '\0');

    fault_clear_all();
    check("clear empties the log",
          fault_snapshot(records, 16) == 0 && fault_active_count() == 0);

    return failed;
}
