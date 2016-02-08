#ifndef _GETTIME_H_
#define _GETTIME_H_

#include <windows.h>
#include "timespec.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CLOCK_REALTIME                  0
#define CLOCK_MONOTONIC                 1
#define CLOCK_PROCESS_CPUTIME_ID        2
#define CLOCK_THREAD_CPUTIME_ID         3
#define CLOCK_MONOTONIC_RAW             4
#define CLOCK_REALTIME_COARSE           5
#define CLOCK_MONOTONIC_COARSE          6
#define CLOCK_BOOTTIME                  7
#define CLOCK_REALTIME_ALARM            8
#define CLOCK_BOOTTIME_ALARM            9
#define CLOCK_SGI_CYCLE                 10      /* Hardware specific */
#define CLOCK_TAI                       11

typedef int clockid_t;

int clock_gettime(clockid_t clk_id, struct timespeca *tp);

//return seconds between start_tv and stop_tv in double precision
double time_since(struct timespeca *start_tv, struct timespeca *stop_tv);

#ifdef  __cplusplus
}
#endif

#endif  //_GETTIME_H_