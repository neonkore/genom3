#
# Copyright (c) 2010 LAAS/CNRS
# All rights reserved.
#
# Redistribution  and  use  in  source  and binary  forms,  with  or  without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of  source  code must retain the  above copyright
#      notice and this list of conditions.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice and  this list of  conditions in the  documentation and/or
#      other materials provided with the distribution.
#
# THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
# WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
# MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
# ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
# WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
# IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#                                           Anthony Mallet on Wed Jun 30 2010
#

namespace eval object {

    # --- foreach ----------------------------------------------------------

    # \proc object foreach {\em var} {\em object} {\em body}
    # \index object foreach
    #
    # Evaluate {\em body} for each element of {\em object}, with {\em var} set
    # to the current element. The result of the command is an empty string.
    #
    # If the {\em object} is of class 'type' (a type object), the exploration
    # of the type tree is done in depth-first order. If {\em body} returns with
    # no error, the next element in explored. If {\em body} returns with
    # 'continue', the descent of the current branch is stopped and the next
    # sibling element is explored. If {\em body} returns with 'break', the
    # procedure returns immediately.
    #
    # Note: when exploring 'type' objects, an endless recursion is possible if
    # the type in question has a recursive definition (e.g. a structure
    # contains a sequence of the same structure). This potentially endless
    # recursion is allowed on purpose, but it is important that you handle this
    # situation in the {\em body} script. A potentially endless recursion can
    # be detected if your {\em body} script encounters either a {\tt forward
    # struct} or a {\tt forward union}. It is up to the caller to determine
    # what to do in this case, but this typically involves returning 'continue'
    # at some point to skip further exploration of that branch.
    #
    # \arg var	A variable name that is set to the current element of object
    #		while iterating.
    # \arg object	A genom object. Must be of class {\tt type}.
    # \arg body	A script evaluated for each element of object.
    #
    proc foreach { var object body } {
	if {[catch {$object class}]} {
	    error "$object is not a genom object"
	}

	switch -- [$object class] {
	    type {
		set e [catch {
		    uplevel \
			[list [namespace code foreach-type] $var $object $body]
		} r opts]
	    }
	}

	if {$e} {
	    switch -- $e {
		0	{ return -options $opts $r }
		1	{ return -options $opts $r }
	    }
	}
	return
    }
    namespace export foreach


    # --- foreach-type -----------------------------------------------------

    # foreach implementation for 'type' objects
    #
    proc foreach-type { var object body } {
	set e [catch {
	    uplevel "set $var [list $object]; eval [list $body]"
	} r opts]
	if {$e} {
	    switch -- $e {
		0	{ return -options $opts $r }
		1	{ return -options $opts $r }
		4	{ return }
	    }
	    return -code 2
	}

	switch -- [$object kind] {
	    {boolean}			-
	    {unsigned short}		-
	    {short}			-
	    {unsigned long}		-
	    {unsigned long long}	-
	    {long}			-
	    {long long}			-
	    {float}			-
	    {double}			-
	    {char}			-
	    {octet}			-
	    {string}			-
	    {enumerator} {
		# basic types: nothing
	    }

	    {enum}			-
	    {struct}			-
	    {union} {
		::foreach m [$object members] {
		    set e [catch {
			uplevel [list [namespace code foreach-type] \
				     $var $m $body]
		    } r opts]
		    if {$e} break
		}
	    }

	    {const}			-
	    {array}			-
	    {sequence}			-
	    {struct member}		-
	    {union member}		-
	    {typedef} {
		set e [catch {
		    uplevel [list [namespace code foreach-type] \
				 $var [$object type] $body]
		} r opts]
	    }

	    {forward struct}		-
	    {forward union} {
		# note: this is deliberately entering a potentially
		# endless recursion if the body script never issue
		# a 'continue' statement to skip this branch.
		set e [catch {
		    uplevel [list [namespace code foreach-type] \
				 $var [$object type] $body]
		} r opts]
	    }

	    default {
		error "internal error: unhandled type '[$object kind]'"
	    }
	}

	if {$e} {
	    switch -- $e {
		0	{ return -options $opts $r }
		1	{ return -options $opts $r }
	    }
	    return -code 2
	}
	return
    }

    namespace ensemble create
}
