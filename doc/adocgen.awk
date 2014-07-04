#!/usr/bin/awk -f
#
# Copyright (c) 2012,2014 LAAS/CNRS
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
#                                            Anthony Mallet on Fri May 18 2012
#

/^[ \t]*\/[*]\// {
    sub(/^[ \t]*\/[*]\/[ \t]*/, "");
    if (!divert()) {
        if (match($0, /[*]\//)) { sub(/[*]\/.*$/, ""); substs(); next; }
        substs();
    }
    grabbing = 1; next
}
/^[ \t]*[#]\// {
    sub(/^[ \t]*[#]\/[ \t]*/, "");
    if (!divert()) substs();
    grabbing = 1; next
}
/^[ \t]*[*#]\/?/ && grabbing {
    sub(/^[ \t]*[*#]\/?[ \t]*/, "");
    if (!divert()) substs();
    next
}

{
    print ""; grabbing = 0
}

END {
    print "// eof";
}

function divert() {
    if (match($0, /^>-$/)) {
        if (diversion) close(diversion)
        diversion = ""
        return 1
    }
    if (match($0, /^>./)) {
        sub(/^>/, ""); diversion = $0
        if (match(diversion, ".*/"))
            system("mkdir -p " substr(diversion, 1, RLENGTH))
        return 1
    }
    if (match($0, /^<./)) {
        sub(/^</, ""); input = $0
        while ((getline < input) > 0)
            substs()
        close(input)
        return 1
    }
    return 0
}

function substs() {
    sub(/@@args/, "@b{Parameters:}\n@quotation\n@table @var")
    sub(/@@end args/, "@end table\n@end quotation")
    sub(/@@returns/, "@b{Returns:}\n@quotation")
    sub(/@@end returns/, "@end quotation")
    if (diversion) print >diversion; else print
}
