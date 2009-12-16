/*
 * Copyright (c) 2009 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and binary  forms,  with  or  without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 * THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
 * WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
 * MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
 * ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
 * WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *                                           Cédric Pasteur on Thu May 14 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>


/* --- local data ---------------------------------------------------------- */

static void	usage(FILE *channel, char *argv0);


/* --- main ---------------------------------------------------------------- */

int
main(int argc, char *argv[])
{
  /* options descriptor */
  static struct option opts[] = {
    { "help",	no_argument,	NULL,	'h' },
    { NULL,	0,		NULL,	0 }
  };

  extern char *optarg;
  extern int optind;
  char *argv0 = argv[0];
  int c;

  while ((c = getopt_long(argc, argv, "h", opts, NULL)) != -1)
    switch (c) {
      case 'h':
	usage(stdout, argv0);
	exit(0);
	break;

      case '?':
      default:
	usage(stderr, argv0);
        exit(1);
        break;
    }
  argc -= optind;
  argv += optind;

  if (argc < 2) {
    usage(stderr, argv0);
    exit(1);
  }

  return 0;
}


/* --- usage --------------------------------------------------------------- */

/** Print usage on a channel.
 *
 * \param[in] channel	Output channel
 * \param[in] argv0	Program invocation name
 */

static void
usage(FILE *channel, char *argv0)
{
  fprintf(channel,
	  "Usage: %s [options] template [options] file\n",
	  basename(argv0));
}
