#ifndef LOGGER_HPP
#define LOGGER_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/timeb.h>
#include <stdio.h>

typedef struct timer {
  struct timeb start_time;
} timer;

void log_to_file(const char *filename);
void end_logging();

void genom_log(const char *format, ...);

void start_timer();
void stop_timer(const char *message);

#ifdef __cplusplus
}
#endif

#endif
