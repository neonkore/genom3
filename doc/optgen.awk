#!/usr/bin/awk -f
#
# Copyright (c) 2010-2012,2014 LAAS/CNRS
# All rights reserved.
#
# Permission to use, copy, modify, and distribute this software for any purpose
# with or without   fee is hereby granted, provided   that the above  copyright
# notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS  SOFTWARE INCLUDING ALL  IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR  BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR  ANY DAMAGES WHATSOEVER RESULTING  FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION,   ARISING OUT OF OR IN    CONNECTION WITH THE USE   OR
# PERFORMANCE OF THIS SOFTWARE.
#
#                                            Anthony Mallet on Sat Jun  5 2010
#

/^(GENERAL|TEMPLATE|ENVIRONMENT) (OPTIONS|VARIABLES)/ {
    argcats[++cats] = substr($0, 1, 1) tolower(substr($0, 2));
    next
}

/^.*::/ {
    if (split($0, f, /::/) != 2) next;
    getline f[2]
    if (f[2] !~ /^\/\//) next;
    gsub(/[*']/, "", f[1]);
    gsub(/\/\/[ \t]*/, "", f[2]);
    argcomm[cats] = argcomm[cats] f[1] SUBSEP f[2] SUBSEP;
    next;
}

END {
    print "static const char usage_string[] = \"\\";
    for (i = 1; i <= cats; i++) {
        print argcats[i] ":\\n\\";
        k = split(argcomm[i], key, SUBSEP);
        for (j = 1; j < k; j+=2) {
            if (key[j] ~ /^--/) { key[j] = "    " key[j] }
            printf("  %-18s %s\\n\\\n", key[j], key[j+1]);
        }

        print "\\n\\";
    }
    print "\";";

    sopt = "";
    lopt = "";
    k = split(argcomm[1], key, SUBSEP);
    for (i = 1; i < k; i+=2) {
        n = split(key[i], opt, /, */)
        short = ""
        for (j = 1; j <= n; j++) {
            if (opt[j] ~ /^-[^-]/) {
                short = substr(opt[j], 2, 1);
                sopt = sopt short;
                if (opt[j] ~ /^-[^-]./ || key[i] ~ /=/) {
                    sopt = sopt ":";
                }
            }
            if (opt[j] ~ /^--[^-]/) {
                match(opt[j], /--[^=]+/)
                long = substr(opt[j], RSTART+2, RLENGTH-2)
                lopt = lopt "\n{ \"" long "\",\t"
                if (opt[j] ~ /=/)
                    lopt = lopt "required"
                else
                    lopt = lopt "no"
                lopt = lopt "_argument,\tNULL,\t"
                if (short == "")
                    lopt = lopt "-'" substr(long, 1, 1) "'"
                else
                    lopt = lopt "'" short "'"
                lopt = lopt "},"
            }
        }
    }

    print "static const char shortopts_string[] = \"+" sopt "\";";
    print "static struct option longopts_list[] = {";
    print   lopt;
    print   "{ NULL, 0, NULL, 0}";
    print "};";
}
