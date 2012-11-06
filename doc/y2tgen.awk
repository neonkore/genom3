#!/usr/bin/awk -f
#
# Copyright (c) 2010-2012 LAAS/CNRS
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
#                                            Anthony Mallet on Sat May  8 2010
#

# rewrite / drop some statements for documentation
/semicolon/ { gsub(/semicolon/, "\";\"") }

/^[^ \t]/ {
    if (n) printdef(n, name, rule)
    n++; name = $1; rule = $0
    next
}

/^[ \t]+\|/ {
    rule = rule "\n" $0; next
}

{
    for (i=1; i<=NF; i++) { rule = rule " " $i; }
}

END { printdef(n, name, rule) }

function printdef(n, name, rule) {
    if (name == "\";\"") return

    out = "dotgen-rule-" name ".texi"
    print "@include " out
    gsub(/_/, "-", rule)
    c = split(rule, l, "\n")
    printf "@verbatim\n%5s " wrap(l[1]) "\n", "(" n ")" > out
    for (i = 2; i <= c; i++) { print "      " wrap(l[i]) >> out }
    print "@end verbatim" >> out
}

function wrap(str,	c, i) {
    match(str, /^[^=|]+[=|]/);
    bs_pln = bs_len = RLENGTH;
    bs_str = substr(str, 1, RLENGTH);
    bs_pre = substr("                                         " \
                    "                                     ", 1, RLENGTH+6);
    c = split(substr(str, RLENGTH + 2), f);
    for (i = 1; i <= c; i++) {
        if (bs_len + 1 + length(f[i]) > 73) {
            bs_str = bs_str "\n" bs_pre; bs_len = bs_pln;
        }
        bs_str = bs_str " " f[i]; bs_len += 1 + length(f[i]);
    }
    return bs_str;
}
