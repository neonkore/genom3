#
# Copyright (c) 2010-2014 LAAS/CNRS
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
#                                           Anthony Mallet on Fri Jun 18 2010
#

namespace eval language::c++ {

    # --- filext -----------------------------------------------------------

    # Return the canonical file extensions in C++
    #
    proc fileext { kind } {
	if {$kind == "header"} { return ".h" } else { return ".cc" }
    }


    # --- comment ----------------------------------------------------------

    # Return a string that is a valid comment in C++.
    #
    proc comment { text } {
      return [concat "//" [join [split $text "\n"] "// "]]
    }


    # --- mapping ----------------------------------------------------------

    # Generate and return the C++ mapping of types matching the glob pattern
    #
    proc mapping { type locations } {
      # Nested types which are defined within the scope of another type trigger
      # the generation of the enclosing type.
      while {![catch {$type parent} p]} { set type $p }

      switch -- [$type kind] {
        {const}		{ append m [genconst $type $locations] }
        {enum}		{ append m [genenum $type $locations] }
        {struct}	{ append m [genstruct $type $locations] }
        {union}		{ append m [genunion $type $locations] }
        {typedef}	{ append m [gentypedef $type $locations] }
        {exception}	{ append m [genexception $type $locations] }

        {forward struct} -
        {forward union}	{ append m [genforward $type $locations] }

        {event}		{ append m [genevent $type $locations] }
        {port}		{ append m [genport $type $locations] }
        {remote}	{ append m [genremote $type $locations] }
        {native}	{ append m [gennative $type $locations] }

        default		{ return "" }
      }

      set p ""
      if {[regexp {u?int(8|16|32|64)_t} $m]} {
        append p "\n#include <stdint.h>"
      }
      if {[regexp {std::string} $m]} {
        append p "\n#include <string>"
      }
      if {[regexp {std::vector<} $m]} {
        append p "\n#include <vector>"
      }
      if {[regexp {genom::bounded_sequence<} $m]} {
        append p "\n#include <genom3/c++/idlsequence.h>"
      }
      if {[regexp {genom::optional<} $m]} {
        append p "\n#include <genom3/c++/idloptional.h>"
      }

      return $p$m
    }


    # --- declarator -------------------------------------------------------

    # Return the C++ mapping of a type declarator.
    #
    proc declarator { type {var {}} } {
	switch -- [$type kind] {
	    {boolean}			{ set d "bool" }
	    {unsigned short}		{ set d "uint16_t" }
	    {short}			{ set d "int16_t" }
	    {unsigned long}		{ set d "uint32_t" }
	    {unsigned long long}	{ set d "uint64_t" }
	    {long}			{ set d "int32_t" }
	    {long long}			{ set d "int64_t" }
	    {float}			{ set d "float" }
	    {double}			{ set d "double" }
	    {char}			{ set d "int8_t" }
	    {octet}			{ set d "uint8_t" }
            {event}			{ set d "genom::event" }
	    {any}			{ error "type any not supported yet" }

            {native}			{ set d "[$type cname] *" }

	    {const}			-
	    {enum}			-
	    {struct}			-
	    {union}			-
	    {typedef}			-
            {exception}			-
            {port}			-
            {remote}			{ set d [$type cname] }

	    {enumerator}		-
	    {struct member}		-
	    {union member}		-
	    {forward struct}		-
	    {forward union}		{ set d [declarator [$type type]] }

	    {string} {
              if {[catch { $type length } l]} {
                set d "std::string"
              } else {
                set d "char\[$l\]"
              }
	    }

	    {array} {
              if {[catch { $type length } l]} { set l {} }
              set d "[declarator [$type type]]"
              set b [string first \[ $d]
              if {$b < 0} {
                append d "\[$l\]"
              } else {
                set d "[string range $d 0 $b-1]\[$l\][string range $d $b end]"
              }
            }

	    {sequence} {
		set t [$type type]
		if {[catch {$type length} l]} {
		    set d "std::vector< [declarator $t] >"
		} else {
		    set d "genom::bounded_sequence< [declarator $t],$l >"
		}
	    }

            optional {
              set t [$type type]
              set d "genom::optional< [declarator $t] >"
            }

	    default {
		template fatal "internal error: unhandled type '[$type kind]'"
	    }
	}

	if {[string length $var] > 0} { set d [cdecl $d $var] }
	return $d
    }


    # --- address ----------------------------------------------------------

    # Return the C++ mapping of a variable address.
    #
    proc address { type {var {}} } {
      switch -- [[$type final] kind] {
        string		{
          if {![catch {[$type final] length}]} { return $var }
        }
        array - native	{ return $var }
      }

      return &($var)
    }


    # --- dereference ------------------------------------------------------

    # Return the C++ mapping for dereferencing a parameter passed by value or
    # reference.
    #
    proc dereference { type kind {var {}} } {
      return $var
    }


    # --- argument --------------------------------------------------------

    # Return the C++ mapping for declaring a parameter.
    #
    proc argument { type kind {var {}} } {
      switch -- $kind {
        {value}		{
          switch -- [[$type final] kind] {
            sequence - optional - struct - union - exception {
              return "const [declarator $type &$var]"
            }
            string {
              if {[catch {[$type final] length}]} {
                return "const [declarator $type &$var]"
              } else {
                return "const [declarator $type $var]"
              }
            }
            array - native {
              return "const [declarator $type $var]"
            }
            port - remote {
              return "[declarator $type &$var]"
            }
            default {
              return [declarator $type $var]
            }
          }
        }

        {reference}	{
          switch -- [[$type final] kind] {
            string {
              if {[catch {[$type final] length}]} {
                return [declarator $type &$var]
              } else {
                return [declarator $type $var]
              }
            }
            array {
              return [declarator $type $var]
            }

            default {
              return [declarator $type &$var]
            }
          }
        }

        default	{
          template fatal \
              "unknown argument kind \"$kind\": must be value or reference"
        }
      }
    }


    # --- pass -------------------------------------------------------------

    # Return the C++ mapping for passing a variable.
    #
    proc pass { type kind {var {}} } {
      switch -- $kind {
        value - reference	{
          return $var
        }
        default {
          template fatal \
              "unknown argument kind \"$kind\": must be value or reference"
        }
      }
    }


    # --- signature --------------------------------------------------------

    # Return the C++ signature of a codel
    #
    proc signature { codel {symchar " "} {location 0}} {
      set ret genom::event
      set sym [cname $codel]
      set arg [list]
      foreach p [$codel parameters] {
        set a ""
        switch -glob -- [$p dir]/[$p src] {
          in/* - */port - */remote {
            append a [[$p type] argument value [$p name]]
          }
          inout/* - out/* {
            append a [[$p type] argument reference [$p name]]
          }
        }
        lappend arg $a
      }
      if {[llength $arg] == 0} { set arg "void" }

      set m ""
      if {$location} {
        set m [genloc $codel]\n
      }
      set p [string repeat " " [string length "${sym}("]]
      set sig [wrap "${sym}([join $arg {, }])" $p ,]
      append m [join [list $ret $sig] $symchar]
      return $m
    }


    # --- member -----------------------------------------------------------

    # Return the C construction to access members of type
    #
    proc member { type mlist } {
      if {[llength $mlist] == 0} return ""
      set mlist [lassign $mlist member]

      switch -- [$type kind] {
        {boolean}				-
        {unsigned short} - {short}		-
        {unsigned long} - {long}		-
        {unsigned long long} - {long long}	-
        {float} - {double}			-
        {char} - {octet}			-
        {enum} - {enumerator} - {string} {
          # basic types: no member
          template fatal "[$type kind] has no member"
        }

        {array} - {sequence} {
          append access "\[$member\]"
          set type [$type type]
        }

        {struct} {
          set m [$type members $member]
          if {[llength $m] == 0} {
            template fatal "[$type kind] has no member $member"
          }
          append access ".$member"
          set type $m
        }

        {union} {
          set m [$type members $member]
          if {[llength $m] == 0} {
            template fatal "[$type kind] has no member $member"
          }
          append access "._u.$member"
          set type $m
        }

        optional {
          set type [$type type]
          append access "._value"
          set mlist [concat $member $mlist]
        }

        {forward struct} - {forward union}	-
        {struct member} - {union member}	-
        {typedef} {
          set type [$type type]
          set mlist [concat $member $mlist]
        }

        default { error "internal error: unhandled type '[$type kind]'" }
      }

      if {[llength $mlist] > 0} {
        append access [member $type $mlist]
      }

      return $access
    }


    # --- invoke -----------------------------------------------------------

    # Return the C++ invocation of a codel
    #
    proc invoke { codel params } {
      if {[llength $params] != [llength [$codel parameters]]} {
        template fatal [subst [join {
          "wrong # arguments in \"[$codel name] invoke\":"
          "[join $params {, }]"
          "expecting:"
          "[signature $codel]"
        } "\n"]]
      }

      set sym [cname $codel]
      return "${sym}([join $params {, }])"
    }


    # --- genconst ---------------------------------------------------------

    # Return the C++ mapping of a const
    #
    proc genconst { type locations } {
      if {[catch {$type fullname}]} return ""
      set t [declarator [$type type] [cname [$type name]]]

      set v [$type value]
      switch [$type valuekind] {
        bool	{ set v [expr {$v?"true":"false"}] }
        char	{ set v "'$v'" }
        string	{ set v "\"[string map {\" \\\"} $v]\"" }
        enum	{ set v [cname $v] }
      }

      if {$locations} { append m [genloc $type] }
      append m [c++namespace "\nconst $t = $v;" $type]
      return [guard $m [$type cname]]
    }


    # --- genenum ----------------------------------------------------------

    # Return the C++ mapping of an enum.
    #
    proc genenum { type locations } {
        if {$locations} { append m [genloc $type] }
	append m "\nenum [cname [$type name]] \{"
	set v -1
	foreach e [$type members] {
            if {$locations} { append m [genloc $e] }
	    append m "\n  [cname [$e name]] =\t[incr v],"
	}
        append m "\n  genom_max_[$type name] =\t0xffffffff"
	append m "\n\};"

	return [guard [c++namespace $m $type] [$type cname]]
    }


    # --- genstruct --------------------------------------------------------

    # Return the C++ mapping of a struct. The typedef is output first to handle
    # recursive structures with anonymous sequences.
    #
    proc genstruct { type locations } {
      set n [cname [$type name]]

      set s "\nstruct $n {"
      # mapping for nested types
      foreach e [$type nested] {
        switch -- [$e kind] {
          struct { append s [genstruct $e $locations] }
          union  { append s [genunion $e $locations] }
          enum   { append s [genenum $e $locations] }
        }
      }

      # member decl
      foreach e [$type members] {
        if {$locations} { append s [genloc $e] }
        append s "\n [declarator $e [$e name]];"
      }
      append s "\n};"

      return [guard [c++namespace $s $type] [$type cname]]
    }


    # --- genunion ---------------------------------------------------------

    # Return the C++ mapping of a union. The typedef is output first to handle
    # recursive and forward decls.
    #
    proc genunion { type locations } {
      set n [cname [$type name]]
      set discr [$type discriminator]

      set s "\nstruct $n {"
      # mapping for nested types
      foreach e [$type nested] {
        switch -- [$e kind] {
          struct { append s [genstruct $e $locations] }
          union  { append s [genunion $e $locations] }
          enum   { append s [genenum $e $locations] }
        }
      }

      # member decl
      if {$locations} { append s [genloc $discr] }
      append s "\n [declarator $discr] _d;"
      append s "\n union {"
      foreach e [$type members] {
        if {$locations} { append s [genloc $e] }
        append s "\n  [declarator $e [$e name]];"

        # C++ does not allow union members with non-trivial ctors
        if {![has_trivial_ctor $e]} {
          set emsg ""
          if {![catch {$e loc} tloc]} {
            lset tloc 0 [file tail [lindex $tloc 0]]
            append emsg [join [lrange $tloc 0 1] :]
            append emsg {: }
          }
          append emsg "member '[$e name]' of type [[$e type] kind]"
          append emsg " not allowed in C++"
          template fatal $emsg
        }
      }
      append s "\n } _u;"
      append s "\n};"

      return [guard [c++namespace $s $type] [$type cname]]
    }


    # --- genexception -----------------------------------------------------

    # Return the C++ mapping of an exception.
    #
    proc genexception { type locations } {
      set n [cname [$type name]]
      append f "\n#include \"genom3/c++/event.h\""

      if {$locations} { append m [genloc $type] }
      append m "\nconst char genom_extern_weak "
      append m "${n}_id\[\] = \"[$type fullname]\";"

      if {$locations} { append m [genloc $type] }
      if {[llength [$type members]]} {
        append m "\nstruct ${n} : public genom::exception {"

        # mapping for nested types
        foreach e [$type nested] {
          switch -- [$e kind] {
            struct { append m [genstruct $e $locations] }
            union  { append m [genunion $e $locations] }
            enum   { append m [genenum $e $locations] }
          }
        }

        # mapping for members
        append m "\n struct detail {"
        foreach e [$type members] {
          if {$locations} { append m [genloc $e] }
          append m "\n  [declarator $e [$e name]];"
        }
        append m "\n } detail;"
        append m "\n const char *what() { return ${n}_id; };"
        append m "\n};"

      } else {
        append m "\nstruct ${n} : public genom::exception {"
        append m "\n typedef void detail;"
        append m "\n const char *what() { return ${n}_id; };"
        append m "\n};"
      }

      return $f[guard [c++namespace $m $type] [$type cname]]
    }


    # --- genforward -------------------------------------------------------

    # Return the C++ mapping of forward declaration.
    #
    proc genforward { type locations } {
      if {$locations} { append m [genloc $type] }
      append m "\nstruct [cname [$type name]];"
      return [guard [c++namespace $m $type] [$type cname]]
    }


    # --- genevent ---------------------------------------------------------

    # Return the C++ mapping of an event.
    #
    proc genevent { type locations } {
      set n [cname [$type name]]
      append f "\n#include \"genom3/c++/event.h\""
      if {$locations} { append m [genloc $type] }
      append m "\nconst char genom_extern_weak $n\[\] = \"[$type fullname]\";"
      return $f[guard [c++namespace $m $type] [$type cname]]
    }


    # --- genport ----------------------------------------------------------

    # Return the C mapping of a port.
    #
    proc genport { type locations } {
      set n [cname [$type name]]
      set p [$type port]
      set t [$p datatype]

      append f "\n#include \"genom3/c++/event.h\""
      if {$locations} { append m [genloc $type] }
      append m "\nstruct $n {"
      switch -- [$p kind]/[$p dir] {
        simple/in {
          append m "\n  virtual const [$t argument reference] data(void)" \
              " const = 0;"
          append m "\n  virtual void read(void) = 0;"
        }
        simple/out {
          append m "\n  virtual [$t argument reference] data(void) const = 0;"
          append m "\n  virtual void write(void) = 0;"
        }

        multiple/in {
          append m \
              "\n  virtual const [$t argument reference] data(const char *id)" \
              " const = 0;"
          append m "\n  virtual void read(const char *id) = 0;"
        }

        multiple/out {
          append m "\n  virtual [$t argument reference] data(const char *id)" \
              " const = 0;"
          append m "\n  virtual void write(const char *id) = 0;"
          append m "\n  virtual void open(const char *id) = 0;"
          append m "\n  virtual void close(const char *id) = 0;"
        }

        default	{ error "invalid port direction" }
      }
      append m "\n};"
      return $f[guard [c++namespace $m $type] [$type cname]]
    }


    # --- genremote --------------------------------------------------------

    # Return the C mapping of a remote.
    #
    proc genremote { type locations } {
      set n [cname [$type name]]
      set r [$type remote]
      append f "\n#include \"genom3/c++/event.h\""

      set arg [list]
      foreach p [$r parameters] {
        switch -- [$p dir] {
          in		{ set a [[$p type] argument value [$p name]] }
          out - inout	{ set a [[$p type] argument reference [$p name]] }
          default	{ template fatal "invalid parameter direction" }
        }
        lappend arg $a
      }
      if {[llength $arg]} { set arg [join $arg {, }] } else { set arg "void" }

      if {$locations} { append m [genloc $type] }
      append m "\nstruct $n {"
      append m "\n  virtual void call($arg) = 0;"
      append m "\n};"
      return $f[guard [c++namespace $m $type] [$type cname]]
    }


    # --- gennative --------------------------------------------------------

    # Return the C mapping of a native.
    #
    proc gennative { type locations } {
      if {$locations} { append m [genloc $type] }
      append m "\nstruct [cname [$type name]];"
      return [guard [c++namespace $m $type] [$type cname]]
    }


    # --- gentypedef -------------------------------------------------------

    # Return the C++ mapping of a typedef.
    #
    proc gentypedef { type locations } {
      if {$locations} { append m [genloc $type] }
      append m "\ntypedef [declarator [$type type] [cname [$type name]]];"
      return [guard [c++namespace $m $type] [$type cname]]
    }


    # --- genloc -----------------------------------------------------------

    # Return the C++ mapping of a source location
    #
    proc genloc { type } {
	if {[catch {$type loc} tloc]} { return "" }

	set f [lindex $tloc 0]
	set l [lindex $tloc 1]
	return "\n#line $l \"[string map {\" \\\"} $f]\""
    }


    # --- has_trivial_ctor -------------------------------------------------

    # Check if the C++ mapping for type has a trivial constructor. Return true
    # if yes.
    #
    proc has_trivial_ctor { type } {
	switch -- [$type kind] {
	    {sequence}		-
            {optional}		-
	    {string}	{ return false }

	    {forward struct}	-
	    {forward union}	-
	    {struct member}	-
	    {union member}	-
	    {array}		-
	    {typedef}	{ return [has_trivial_ctor [$type type]] }

	    {struct}		-
	    {union}	{
		foreach m [$type members] {
		    if {![has_trivial_ctor $m]} { return false }
		}
		return true
	    }

	    default	{ return true }
	}
    }


    # --- c++namespace -----------------------------------------------------

    # Generate namespace hiearachy for type
    #
    proc c++namespace { string type } {
      foreach s [$type scope] {
        # nested structs or union are defined by the enclosing type and need
        # no namespace.
        if {[lindex $s 0] ne "module"} { return $string }

        append m "\nnamespace [lindex $s 1] \{"
      }
      append m "$string"
      foreach s [$type scope] {
        append m "\n\}"
      }
      return $m
    }

    # --- guard -----------------------------------------------------------

    # Protect a string with guards
    #
    proc guard { string name } {
	set name [string map {:: _} $name]
	append m "\n#ifndef _${name}_cxx_type"
	append m "\n# define _${name}_cxx_type"
	append m $string
	append m "\n#endif /* _${name}_cxx_type */"
	append m "\n"
	return $m
    }


    # --- cdecl ------------------------------------------------------------

    # Given a type string and a name, declare name of type type. The subtlety
    # comes from arrays, because name should preceede the brackets, if any.
    #
    proc cdecl { type name } {
	set b [string first \[ $type]
	if { $b < 0 } {
	    if {[string index $type end] eq "*"} {
		return "$type$name"
	    }
	    return "$type $name"
	}

	return "[string range $type 0 $b-1] $name[string range $type $b end]"
    }


    # --- cname ------------------------------------------------------------

    # Cannonical name of an object or string in C++.
    # Remove leading :: and map other :: to _
    #
    proc cname { object } {
	if {![catch {$object class} class]} {
	    switch -- $class {
              codel	{ set object [$object name] }
              type	{ set object [$object fullname] }
	    }
	}

	if { [string first :: $object] == 0 } {
	    set object [string range $object 2 end]
	}
	return [string map {{ } _ + x} $object]
    }
}
