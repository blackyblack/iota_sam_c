#include <time.h>
#include "gettime.h"

#ifdef  __cplusplus
extern "C" {
#endif

static LARGE_INTEGER getFILETIMEoffset()
{
  LARGE_INTEGER t;
  ///HACK: offset to move time from 01.01.1601 to 01.01.1970
  t.QuadPart = 116444736000000000ULL;
  return t;
}

int clock_gettime(clockid_t clk_id, struct timespeca *tp)
{
  LARGE_INTEGER           t;
  FILETIME                f;
  static LARGE_INTEGER    offset;
  static double           frequencyToMicroseconds;
  static int              initialized = 0;
  static BOOL             usePerformanceCounter = 0;

  if (!initialized) {
    LARGE_INTEGER performanceFrequency;
    initialized = 1;
    if (clk_id == CLOCK_MONOTONIC)
    {
      usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
    }
    if (usePerformanceCounter) {
      QueryPerformanceCounter(&offset);
      frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
    }
    else {
      offset = getFILETIMEoffset();
      frequencyToMicroseconds = 10.;
    }
  }
  if (usePerformanceCounter) QueryPerformanceCounter(&t);
  else {
    GetSystemTimeAsFileTime(&f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
  }

  t.QuadPart -= offset.QuadPart;
  t.QuadPart /= (LONGLONG)frequencyToMicroseconds;
  tp->tv_sec = t.QuadPart / 1000000ULL;
  tp->tv_nsec = (t.QuadPart % 1000000ULL) * 1000;
  return (0);
}

//return seconds between start_tv and stop_tv in double precision
double time_since(struct timespeca *start_tv, struct timespeca *stop_tv)
{
  double sec, usec;
  double ret;
  sec = (double)(stop_tv->tv_sec - start_tv->tv_sec);
  usec = (double)((stop_tv->tv_nsec - start_tv->tv_nsec) / 1000);
  if (sec > 0 && usec < 0) {
    sec--;
    usec += 1e6;
  }
  ret = sec + usec*1e-6;
  if (ret < 0)
    ret = 0;
  return ret;
}

#ifdef  __cplusplus
}
#endif