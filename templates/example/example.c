<'
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
#                                            Anthony Mallet on Tue Jun  8 2010
#

# This file is a template source file, for the "example" genom3 template.

# Any template source file may contain < ' ' > marker pairs (without the
# spaces, like the markers surrounding this text). The text between the two
# markers is evaluated (i.e. executed) by the Tcl interpreter as a script. The
# markers and the script source is then discarded and replaced by whatever the
# scripts outputs (e.g. with puts). The result of the script itself is
# discarded.

# In the case of this text, nothing will be output to the generated file
# since the script only consists of Tcl comments or blank line, which
# output nothing.

# This is useful for commenting the template source itself but not the
# generated file, like for insterting the copyright statement above.
'>

<'
# Any text that is not enclosed between < ' ' > markers is copied verbatim
# to the generated file. For instance, the blank line above and below will be
# output as-is.
'>

<'
# A template source file can also contain < " " > markers (again, without the
# spaces). In this case, the text enclosed within the markers is passed to
# the "subst" command of Tcl. This command will substitute all variable and
# command references. The result of this substitution will replace the
# original text and the markers. In this case, any text that is output by e.g.
# puts is NOT output.

# Any side effects of the substition will modify the current state of the
# interpreter. For instance, the < " [incr i] " > substition will actually
# increment the variable i, and the value of i will remain accessible from
# other scripts in this file.
'>

<'
# As an exercise, we will now output, as a C comment, the copyright notice
# present in the .gen source file, by using < " " > markers and the
# "dotgen input notice" template procedure:
'>
/* <"[dotgen input notice]"> */

<' # We could have used the "language comment" procedure as well:'>
<"[language comment c [dotgen input notice]]">

<' # I am running out of time, so I stop this tutorial here for now... '>

#include <stdio.h>

<' set name [[dotgen component] name] '>

int
main(int argc, char *argv[])
{
  printf("%s\n", <"[example_quote $name]">);
  return 0;
}
