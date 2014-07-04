/* This file is public domain. It computes the latest modification date of a
 * list of files. This overcomes the fact that stat(1) output vary too much
 * across systems, and is thus unusable. stat(2) is standardized. */
#include "acgenom.h"

#include <sys/stat.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(int argc, const char *argv[])
{
  char date[128];
  struct stat sb;
  struct tm *tm;
  time_t t = 0;

  if (argc < 2) errx(2, "usage: %s file [file ...]", argv[0]);
  putenv("LANG=C");
  putenv("LC_TIME=C");

  for(argv++; *argv; argv++) {
    if (stat(*argv, &sb)) err(2, "%s", argv[0]);
    if (t < sb.st_mtime) t = sb.st_mtime;
  }

  tm = localtime(&t);
  if (!tm) err(2, "localtime");

  if (!strftime(date, sizeof(date), "%Y", tm)) err(2, "strftime");
  printf("YEAR='%s'\n", date);
  if (!strftime(date, sizeof(date), "%B", tm)) err(2, "strftime");
  printf("MONTH='%s'\n", date);
  return 0;
}
