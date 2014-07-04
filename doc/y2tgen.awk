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
#                                            Anthony Mallet on Sat May  8 2010
#

# rewrite / drop some statements for documentation
/semicolon/ { gsub(/semicolon/, "\";\"") }

/^[^ \t]/ {
    if (n) {
        gsub(/_/, "-", name)
        gsub(/_/, "-", rule)
        names[n] = name
        rules[name] = rule
    }
    n++; name = $1; rule = $0
    next
}

/^[ \t]+\|/ {
    rule = rule $0; next
}

{
    for (i=1; i<=NF; i++) { rule = rule " " $i; }
}

END {
    names[n] = name
    rules[n] = rule

    print "Grammar reference"
    print "-----------------"
    print ""

    for (i=1; i<=n; i++) {
        printdef(i, names[i], rules[names[i]])
    }

    print "// eof"
}

function printdef(n, name, rule) {
    if (name == "\";\"") return

    out = "dotgen-rule-" name ".adoc"
    r = "[[dotgen-rule-" name "]]\n____"
    print r
    print r > out

    r = sprintf("%s " wrap(rule) "\n____", "(" n ")")
    print r
    print r > out
    close(out)
}

function wrap(str,	c, i) {
    match(str, /^[^=|]+[=|]/);
    bs_pln = bs_len = RLENGTH;
    bs_str = substr(str, 1, RLENGTH) " ::\n  ";
    bs_pre = substr("                                          " \
                    "                                     ", 1, RLENGTH+4);
    c = split(substr(str, RLENGTH + 2), f);
    for (i = 1; i <= c; i++) {
        if (f[i] in rules) {
            md = "link:grammar{outfilesuffix}#dotgen-rule-" f[i] "[" f[i] "]";
        } else {
            md = f[i];
        }
        if (f[i] == "|") {
            bs_str = bs_str " +\n" bs_pre; bs_len = bs_pln;
        }
        bs_str = bs_str " " md; bs_len += 1 + length(f[i]);
    }
    return bs_str;
}
