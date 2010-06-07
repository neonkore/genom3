#!/usr/bin/awk -f
#
# Copyright (c) 2010 LAAS/CNRS
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
#                                            Anthony Mallet on Sun May  9 2010
#
BEGIN {
    NONE = 0;
    ARG = 1;
    RET = 2;
    DOC = 3;
    SEC = 4;
    grabbing = NONE;
}

/\\proc/ {
    sub(/^.*\\proc[ \t]+/, "");
    print "\\begin{engcmd}{" $0 "}";
    grabbing = DOC;
    next;
}

/\\section/ {
    sub(/^.*\\section[ \t]+/, "");
    print "\\subsection{" $0 "}";
    grabbing = SEC;
    next;
}

/\\index/ {
    sub(/^.*\\index[ \t]+/, "");
    for (j = 1; j <= NF; j++) {
	printf "\\index{";
	printf "%s@\\texttt{%s} (template command)", $1, $1;
	for (i = 2; i <= j; i++) {
	    printf "!%s@\\texttt{%s}", $i, $i;
	}
	print "}";
    }
    printf "\\index{template command!%s@\\texttt{%s}", $1, $1;
    print "}";
    next;
}

/\\arg/ && grabbing != NONE {
    if (grabbing != DOC) {
	print "}";
    }
    sub(/^.*\\arg[ \t]+/, "");
    print "\\engcmdarg{" $1 "}{" substr($0, index($0, $2));
    grabbing = ARG;
    next;
}

/\\return/ && grabbing != NONE {
    if (grabbing != DOC) {
	print "}";
    }
    sub(/^.*\\return[ \t]+/, "");
    print "\\engcmdret{" $0;
    grabbing = RET;
    next;
}

/^[ \t]*[#*]+[ \t]*$/ && grabbing != NONE && grabbing != SEC{
    if (grabbing != DOC) {
	print "}";
    }
    print "";
    grabbing = DOC;
    next;
}

/^[ \t]*[#*]/ && grabbing != NONE {
    sub(/^[ \t]*[#*]+[ \t\/]*/, "");
    print $0;
    next;
}

grabbing == SEC {
    grabbing = NONE;
}

grabbing != NONE {
    if (grabbing != DOC) {
	print "}";
    }
    print "\\end{engcmd}\n";
    grabbing = NONE;
}
