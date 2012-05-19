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
#                                            Anthony Mallet on Sat Jun  5 2010
#
BEGIN {
    grabbing = "none";
    output = "c";

    for (i=1; i<ARGC; i++) {
	if (ARGV[i] ~ /^-c/)
	    output = "c";
	else if (ARGV[i] ~ /^-t/)
	    output = "texi";
	else if (ARGV[i] ~ /^-m/)
	    output = "mdoc";
	else if (ARGV[i] ~ /^-./) {
	    print "Unknown option " ARGV[i];
	    exit;
	} else
	    break;
    }
    for (j=1; i<ARGC; i++ && j++)
	ARGV[j] = ARGV[i];
    ARGC = j;
}

/^#/ {
    grabbing = "none";
}

/^\\args/ {
    grabbing = "none";
    t = substr($0, index($0, $3))
    argcat = $2;
    arglist = arglist " " argcat;
    args[argcat] = t;
    next;
}

/^\\arg/ {
    grabbing = substr($1, 2);
    argkey = $2;
    argkeys[argcat] = argkeys[argcat] " " argkey;
    arglong[argcat,argkey] = $3;
    argopt[argcat,argkey] = $4;
    next;
}

/^\\env/ {
    grabbing = substr($1, 2);
    envkey = $2
    envs = envs " " envkey;
    next;
}

/^\\desc/ {
    grabbing = substr($1, 2);
    next;
}

grabbing ~ "arg" {
    sub(/^[ \t]+/, ""); sub(/[ \t]+$/, "");
    if (!argcomm[argcat,argkey])
	argcomm[argcat,argkey] = $0;
    else
	argdesc[argcat,argkey] = argdesc[argcat,argkey] "\n" $0;
}

grabbing ~ "env" {
    sub(/^[ \t]+/, ""); sub(/[ \t]+$/, "");
    if (!envcomm[envkey])
	envcomm[envkey] = $0;
    else
	envdesc[envkey] = envdesc[envkey] "\n" $0;
}

grabbing ~ "desc" {
    sub(/^[ \t]+/, ""); sub(/[ \t]+$/, "");
    desc = desc "\n" $0;
}

END {
    sub(/^[ \t\n]+/, "", desc); sub(/[ \t\n]+$/, "", desc);
    for (k in argdesc) {
	sub(/^[ \t\n]+/, "", argdesc[k]); sub(/[ \t\n]+$/, "", argdesc[k]);
    }
    for (k in envdesc) {
	sub(/^[ \t\n]+/, "", envdesc[k]); sub(/[ \t\n]+$/, "", envdesc[k]);
    }

    if (output == "c")
	output_c();
    if (output == "texi")
	output_texi();
    if (output == "mdoc")
	output_mdoc();
}

function output_c() {
    print "static const char usage_string[] = \"\\";
    c = split(arglist, cat);
    for (i = 1; i <= c; i++) {
	print args[cat[i]] ":\\n\\";
	k = split(argkeys[cat[i]], key);
	for (j = 1; j <= k; j++) {
	    if (key[j] ~ /^-/)
		s = "    ";
	    else
		s = "  -" key[j];
	    if (arglong[cat[i],key[j]] != "-") {
		if (key[j] ~ /^-/)
		    s = s " ";
		else
		    s = s ",";
		s = s "--" arglong[cat[i],key[j]];
		if (argopt[cat[i],key[j]])
		    s = s "=" argopt[cat[i],key[j]];
	    } else {
		if (argopt[cat[i],key[j]])
		    s = s argopt[cat[i],key[j]];
	    }
	    while(length(s) < 20)
		s = s " ";
	    print s argcomm[cat[i],key[j]] "\\n\\";
	}
	print "\\n\\";
    }
    print "Environment variables:\\n\\";
    k = split(envs, key);
    for (i = 1; i <= k; i++) {
	s = "  " key[i];
	while(length(s) < 20)
	    s = s " ";
	print s envcomm[key[i]] "\\n\\";
    }
    print "\";";

    printf "static const char shortopts_string[] = \"+";
    split(argkeys["general"], key);
    for (k in key) {
	if (key[k] !~ /^-/) {
	    printf "%s", key[k];
	    if (argopt["general",key[k]])
		printf ":";
	}
    }
    print "\";";

    print "static struct option longopts_list[] = {";
    split(argkeys["general"], key);
    for (k in key) {
	if (arglong["general",key[k]] != "-") {
	    printf "{ \"%s\",\t", arglong["general",key[k]];
	    if (argopt["general",key[k]])
		printf "required_argument,\t";
	    else
		printf "no_argument,\t";
	    printf "NULL,\t";
	    if (key[k] !~ /^-/)
		printf "'%s'", key[k];
	    else
		printf "%s", key[k];
	    print "},";
	}
    }
    print "{ NULL, 0, NULL, 0}";
    print "};";
}


function output_texi() {
    print "@node Description";
    print "@section Description";
    gsub(/\\name/, "@code{genom3}", desc);
    gsub(/\\em[ \t]*/, "@i{",  desc);
    gsub(/\\tt[ \t]*/, "@code{",  desc);
    gsub(/[ \t]*\\rm/, "}",  desc);
    print desc;

    c = split(arglist, cat);
    for (i = 1; i <= c; i++) {
	print "@node " args[cat[i]];
	print "@section " args[cat[i]]
	print "@table @code";
	k = split(argkeys[cat[i]], key);
	for (j = 1; j <= k; j++) {
	    printf "@item ";
	    if (key[j] ~ /^-/)
		s = "";
	    else {
		s = "-" key[j];
                if (argopt[cat[i],key[j]])
                    s = s " @i{" argopt[cat[i],key[j]] "}";
            }
	    if (arglong[cat[i],key[j]] != "-") {
		if (key[j] !~ /^-/)
		    s = s "\n@itemx ";
		s = s "--" arglong[cat[i],key[j]];
		if (argopt[cat[i],key[j]])
		    s = s "=@i{" argopt[cat[i],key[j]] "}";
	    }
	    print s;

	    gsub(/\\em[ \t]*/, "@var{", argdesc[cat[i],key[j]]);
	    gsub(/\\tt[ \t]*/, "@code{", argdesc[cat[i],key[j]]);
	    gsub(/[ \t]*\\rm/, "}", argdesc[cat[i],key[j]]);
	    print argdesc[cat[i],key[j]];
	    print "";
	}
	print "@end table";
    }

    print "@node Environment variables";
    print "@section Environment variables";
    print "@table @code";
    k = split(envs, key);
    for (i = 1; i <= k; i++) {
        s = key[i]
	print "@item " s "";
	gsub(/\\name/, "@genom{}", envdesc[key[i]]);
	gsub(/\\em[ \t]*/, "@env{", envdesc[key[i]]);
	gsub(/\\tt[ \t]*/, "@code{", envdesc[key[i]]);
	gsub(/[ \t]*\\rm/, "}", envdesc[key[i]]);
	print envdesc[key[i]];
    }
    print "@end table";
}

function output_mdoc() {
    print ".Sh DESCRIPTION";
    gsub(/^\\name/, ".Nm\n.Ns ", desc);
    gsub(/\n?\\name/, "\n.Nm\n.Ns ", desc);
    gsub(/\n?\\em/, "\n.Em", desc);
    gsub(/\n?\\tt/, "\n.Em", desc);
    gsub(/\n?\\rm/, "\n.Ns ", desc);
    gsub(/\n\n/, "\n.Pp\n", desc);
    print desc;

    c = split(arglist, cat);
    for (i = 1; i <= c; i++) {
      print ".Sh " toupper(args[cat[i]]);
      print ".Bl -tag -width pancake";
      k = split(argkeys[cat[i]], key);
      for (j = 1; j <= k; j++) {
	printf ".It ";
	if (key[j] ~ /^-/)
	  s = "";
	else {
	  s = "Fl " key[j];
          if (argopt[cat[i],key[j]])
              s = s " Em " argopt[cat[i],key[j]];
        }
	if (arglong[cat[i],key[j]] != "-") {
	  if (key[j] !~ /^-/)
	    s = s " ,";
	  s = s " Fl -" arglong[cat[i],key[j]];
	  if (argopt[cat[i],key[j]])
	    s = s " Ns = Ns Em " argopt[cat[i],key[j]];
	}
	print s;
	gsub(/\n?\\em/, "\n.Em", argdesc[cat[i],key[j]]);
	gsub(/\n?\\tt/, "\n.Li", argdesc[cat[i],key[j]]);
	gsub(/\n?\\rm/, "\n.Ns ", argdesc[cat[i],key[j]]);
	gsub(/\n\n/, "\n.Pp\n", argdesc[cat[i],key[j]]);
	print argdesc[cat[i],key[j]];
      }
      print ".El";
    }

    print ".Sh ENVIRONMENT";
    print ".Bl -tag -width TMPDIR";
    k = split(envs, key);
    for (i = 1; i <= k; i++) {
	print ".It Ev " key[i];
	gsub(/\n?\\name/, "\n.Nm\n.Ns ", envdesc[key[i]]);
	gsub(/\n?\\em/, "\n.Em", envdesc[key[i]]);
	gsub(/\n?\\tt/, "\n.Li", envdesc[key[i]]);
	gsub(/\n?\\rm/, "\n.Ns ", envdesc[key[i]]);
	gsub(/\n\n/, "\n.Pp\n", envdesc[key[i]]);
	print envdesc[key[i]];
    }
    print ".El";
}
