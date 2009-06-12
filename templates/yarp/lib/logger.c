#include <stdarg.h>
#include <stdlib.h>

#include "logger.h"

timer global_timer;
FILE* logging_file = NULL;

void log_to_file(const char *filename)
{
  logging_file = fopen(filename, "a+");
  if(logging_file == NULL) {
    fprintf(stderr, "Impossible to open the file '%s'", filename);
    return;
  }

  fprintf(logging_file, "\n\n*******************************************************\n");
  fprintf(logging_file, "****************** Start logging **********************\n");
  fprintf(logging_file, "*******************************************************\n");
}

void genom_log(const char *message, ...)
{
  va_list ap;
  FILE *out = logging_file ? logging_file : stdout;
  
  struct timeb time;
  ftime(&time);
  fprintf(out, "[%d.%d] :", time.time, time.millitm, message);

  va_start(ap, message);
  vfprintf(out, message, ap);
  va_end(ap);
  
  fprintf(out, "\n");
}

void end_logging()
{
  if(logging_file != NULL) { 
    fprintf(logging_file, "\n\n*******************************************************\n");
    fprintf(logging_file, "****************** Start logging **********************\n");
    fprintf(logging_file, "*******************************************************\n");
    fclose(logging_file);
  }
}

void start_timer()
{
  ftime(&global_timer.start_time);
}

void stop_timer(const char *message)
{
  struct timeb end;
  ftime(&end);
  genom_log("%s duration: %d.%d s\n", message, end.time - global_timer.start_time.time,
      end.millitm - global_timer.start_time.millitm);
}
