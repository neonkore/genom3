# md5.tcl - Copyright (C) 2003 Pat Thoyts <patthoyts@users.sourceforge.net>
#
# MD5  defined by RFC 1321, "The MD5 Message-Digest Algorithm"
#
# This is an implementation of MD5 based upon the example code given in
# RFC 1321 and upon the tcllib MD4 implementation and taking some ideas
# from the earlier tcllib md5 version by Don Libes.
#
# This implementation permits incremental updating of the hash.
#
# This software is copyrighted by Ajuba Solutions and other parties.
# The following terms apply to all files associated with the software unless
# explicitly disclaimed in individual files.
#
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.
#
# IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
# DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT. THIS SOFTWARE
# IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
# NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
# MODIFICATIONS.
#
# GOVERNMENT USE: If you are acquiring this software on behalf of the
# U.S. government, the Government shall have only "Restricted Rights"
# in the software and related documentation as defined in the Federal
# Acquisition Regulations (FARs) in Clause 52.227.19 (c) (2). If you
# are acquiring the software on behalf of the Department of Defense, the
# software shall be classified as "Commercial Computer Software" and the
# Government shall have only "Restricted Rights" as defined in Clause
# 252.227-7013 (c) (1) of DFARs. Notwithstanding the foregoing, the
# authors grant the U.S. Government and others acting in its behalf
# permission to use and distribute the software in accordance with the
# terms specified in this license.
#
# From $Id: md5x.tcl,v 1.19 2008/07/04 18:27:00 andreas_kupries Exp $

namespace eval md5 {

  # --- init ---------------------------------------------------------------
  #
  # Create and initialize an MD5 state variable. This will be
  # cleaned up when we call MD5Final
  #
  proc init {} {
    variable state

    # RFC1321:3.3 - Initialize MD5 state structure
    array set state \
        [list \
             A [expr {0x67452301}] \
             B [expr {0xefcdab89}] \
             C [expr {0x98badcfe}] \
             D [expr {0x10325476}] \
             n 0 i "" ]
  }


  # --- update -------------------------------------------------------------
  #
  # This is called to add more data into the hash. You may call this
  # as many times as you require. Note that passing in "ABC" is NOT equivalent
  # to passing these letters in as separate calls
  #
  proc update {data} {
    variable state

    # Chunk separator
    append data |

    # Update the state values
    incr state(n) [string length $data]
    append state(i) $data

    # Calculate the hash for any complete blocks
    set len [string length $state(i)]
    for {set n 0} {($n + 64) <= $len} {} {
      MD5Hash [string range $state(i) $n [incr n 64]]
    }

    # Adjust the state for the blocks completed.
    set state(i) [string range $state(i) $n end]
    return
  }


  # --- final --------------------------------------------------------------
  #
  # This procedure is used to close the current hash and returns the
  # hash data. Once this procedure has been called the hash context
  # is freed and cannot be used again.
  #
  # Note that the output is 128 bits represented as 32 hexadecimal digits.
  #
  proc final {} {
    variable state

    # RFC1321:3.1 - Padding
    #
    set len [string length $state(i)]
    set pad [expr {56 - ($len % 64)}]
    if {$len % 64 > 56} {
      incr pad 64
    }
    if {$pad == 0} {
      incr pad 64
    }
    append state(i) [binary format a$pad \x80]

    # RFC1321:3.2 - Append length in bits as little-endian wide int.
    append state(i) [binary format ii [expr {8 * $state(n)}] 0]

    # Calculate the hash for the remaining block.
    set len [string length $state(i)]
    for {set n 0} {($n + 64) <= $len} {} {
      MD5Hash [string range $state(i) $n [incr n 64]]
    }

    # RFC1321:3.5 - Output
    set r [bytes $state(A)][bytes $state(B)][bytes $state(C)][bytes $state(D)]
    unset state
    return $r
  }


  # --- MD5Hash ------------------------------------------------------------
  #
  # This is the core MD5 algorithm. It is a lot like the MD4 algorithm but
  # includes an extra round and a set of constant modifiers throughout.
  #
  # Note:
  #  This function body is substituted later on to inline some of the
  #  procedures and to make is a bit more comprehensible.
  #
  set body {
    variable state

    # RFC1321:3.4 - Process Message in 16-Word Blocks
    binary scan $msg i* blocks
    foreach {X0 X1 X2 X3 X4 X5 X6 X7 X8 X9 X10 X11 X12 X13 X14 X15} $blocks {
      set A $state(A)
      set B $state(B)
      set C $state(C)
      set D $state(D)

      # Round 1
      # Let [abcd k s i] denote the operation
      #   a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s).
      # Do the following 16 operations.
      # [ABCD  0  7  1]  [DABC  1 12  2]  [CDAB  2 17  3]  [BCDA  3 22  4]
      set A [expr {$B + (($A + [F $B $C $D] + $X0 + $T01) <<< 7)}]
      set D [expr {$A + (($D + [F $A $B $C] + $X1 + $T02) <<< 12)}]
      set C [expr {$D + (($C + [F $D $A $B] + $X2 + $T03) <<< 17)}]
      set B [expr {$C + (($B + [F $C $D $A] + $X3 + $T04) <<< 22)}]
      # [ABCD  4  7  5]  [DABC  5 12  6]  [CDAB  6 17  7]  [BCDA  7 22  8]
      set A [expr {$B + (($A + [F $B $C $D] + $X4 + $T05) <<< 7)}]
      set D [expr {$A + (($D + [F $A $B $C] + $X5 + $T06) <<< 12)}]
      set C [expr {$D + (($C + [F $D $A $B] + $X6 + $T07) <<< 17)}]
      set B [expr {$C + (($B + [F $C $D $A] + $X7 + $T08) <<< 22)}]
      # [ABCD  8  7  9]  [DABC  9 12 10]  [CDAB 10 17 11]  [BCDA 11 22 12]
      set A [expr {$B + (($A + [F $B $C $D] + $X8 + $T09) <<< 7)}]
      set D [expr {$A + (($D + [F $A $B $C] + $X9 + $T10) <<< 12)}]
      set C [expr {$D + (($C + [F $D $A $B] + $X10 + $T11) <<< 17)}]
      set B [expr {$C + (($B + [F $C $D $A] + $X11 + $T12) <<< 22)}]
      # [ABCD 12  7 13]  [DABC 13 12 14]  [CDAB 14 17 15]  [BCDA 15 22 16]
      set A [expr {$B + (($A + [F $B $C $D] + $X12 + $T13) <<< 7)}]
      set D [expr {$A + (($D + [F $A $B $C] + $X13 + $T14) <<< 12)}]
      set C [expr {$D + (($C + [F $D $A $B] + $X14 + $T15) <<< 17)}]
      set B [expr {$C + (($B + [F $C $D $A] + $X15 + $T16) <<< 22)}]

      # Round 2.
      # Let [abcd k s i] denote the operation
      #   a = b + ((a + G(b,c,d) + X[k] + Ti) <<< s)
      # Do the following 16 operations.
      # [ABCD  1  5 17]  [DABC  6  9 18]  [CDAB 11 14 19]  [BCDA  0 20 20]
      set A [expr {$B + (($A + [G $B $C $D] + $X1  + $T17) <<<  5)}]
      set D [expr {$A + (($D + [G $A $B $C] + $X6  + $T18) <<<  9)}]
      set C [expr {$D + (($C + [G $D $A $B] + $X11 + $T19) <<< 14)}]
      set B [expr {$C + (($B + [G $C $D $A] + $X0  + $T20) <<< 20)}]
      # [ABCD  5  5 21]  [DABC 10  9 22]  [CDAB 15 14 23]  [BCDA  4 20 24]
      set A [expr {$B + (($A + [G $B $C $D] + $X5  + $T21) <<<  5)}]
      set D [expr {$A + (($D + [G $A $B $C] + $X10 + $T22) <<<  9)}]
      set C [expr {$D + (($C + [G $D $A $B] + $X15 + $T23) <<< 14)}]
      set B [expr {$C + (($B + [G $C $D $A] + $X4  + $T24) <<< 20)}]
      # [ABCD  9  5 25]  [DABC 14  9 26]  [CDAB  3 14 27]  [BCDA  8 20 28]
      set A [expr {$B + (($A + [G $B $C $D] + $X9  + $T25) <<<  5)}]
      set D [expr {$A + (($D + [G $A $B $C] + $X14 + $T26) <<<  9)}]
      set C [expr {$D + (($C + [G $D $A $B] + $X3  + $T27) <<< 14)}]
      set B [expr {$C + (($B + [G $C $D $A] + $X8  + $T28) <<< 20)}]
      # [ABCD 13  5 29]  [DABC  2  9 30]  [CDAB  7 14 31]  [BCDA 12 20 32]
      set A [expr {$B + (($A + [G $B $C $D] + $X13 + $T29) <<<  5)}]
      set D [expr {$A + (($D + [G $A $B $C] + $X2  + $T30) <<<  9)}]
      set C [expr {$D + (($C + [G $D $A $B] + $X7  + $T31) <<< 14)}]
      set B [expr {$C + (($B + [G $C $D $A] + $X12 + $T32) <<< 20)}]

      # Round 3.
      # Let [abcd k s i] denote the operation
      #   a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s)
      # Do the following 16 operations.
      # [ABCD  5  4 33]  [DABC  8 11 34]  [CDAB 11 16 35]  [BCDA 14 23 36]
      set A [expr {$B + (($A + [H $B $C $D] + $X5  + $T33) <<<  4)}]
      set D [expr {$A + (($D + [H $A $B $C] + $X8  + $T34) <<< 11)}]
      set C [expr {$D + (($C + [H $D $A $B] + $X11 + $T35) <<< 16)}]
      set B [expr {$C + (($B + [H $C $D $A] + $X14 + $T36) <<< 23)}]
      # [ABCD  1  4 37]  [DABC  4 11 38]  [CDAB  7 16 39]  [BCDA 10 23 40]
      set A [expr {$B + (($A + [H $B $C $D] + $X1  + $T37) <<<  4)}]
      set D [expr {$A + (($D + [H $A $B $C] + $X4  + $T38) <<< 11)}]
      set C [expr {$D + (($C + [H $D $A $B] + $X7  + $T39) <<< 16)}]
      set B [expr {$C + (($B + [H $C $D $A] + $X10 + $T40) <<< 23)}]
      # [ABCD 13  4 41]  [DABC  0 11 42]  [CDAB  3 16 43]  [BCDA  6 23 44]
      set A [expr {$B + (($A + [H $B $C $D] + $X13 + $T41) <<<  4)}]
      set D [expr {$A + (($D + [H $A $B $C] + $X0  + $T42) <<< 11)}]
      set C [expr {$D + (($C + [H $D $A $B] + $X3  + $T43) <<< 16)}]
      set B [expr {$C + (($B + [H $C $D $A] + $X6  + $T44) <<< 23)}]
      # [ABCD  9  4 45]  [DABC 12 11 46]  [CDAB 15 16 47]  [BCDA  2 23 48]
      set A [expr {$B + (($A + [H $B $C $D] + $X9  + $T45) <<<  4)}]
      set D [expr {$A + (($D + [H $A $B $C] + $X12 + $T46) <<< 11)}]
      set C [expr {$D + (($C + [H $D $A $B] + $X15 + $T47) <<< 16)}]
      set B [expr {$C + (($B + [H $C $D $A] + $X2  + $T48) <<< 23)}]

      # Round 4.
      # Let [abcd k s i] denote the operation
      #   a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s)
      # Do the following 16 operations.
      # [ABCD  0  6 49]  [DABC  7 10 50]  [CDAB 14 15 51]  [BCDA  5 21 52]
      set A [expr {$B + (($A + [I $B $C $D] + $X0  + $T49) <<<  6)}]
      set D [expr {$A + (($D + [I $A $B $C] + $X7  + $T50) <<< 10)}]
      set C [expr {$D + (($C + [I $D $A $B] + $X14 + $T51) <<< 15)}]
      set B [expr {$C + (($B + [I $C $D $A] + $X5  + $T52) <<< 21)}]
      # [ABCD 12  6 53]  [DABC  3 10 54]  [CDAB 10 15 55]  [BCDA  1 21 56]
      set A [expr {$B + (($A + [I $B $C $D] + $X12 + $T53) <<<  6)}]
      set D [expr {$A + (($D + [I $A $B $C] + $X3  + $T54) <<< 10)}]
      set C [expr {$D + (($C + [I $D $A $B] + $X10 + $T55) <<< 15)}]
      set B [expr {$C + (($B + [I $C $D $A] + $X1  + $T56) <<< 21)}]
      # [ABCD  8  6 57]  [DABC 15 10 58]  [CDAB  6 15 59]  [BCDA 13 21 60]
      set A [expr {$B + (($A + [I $B $C $D] + $X8  + $T57) <<<  6)}]
      set D [expr {$A + (($D + [I $A $B $C] + $X15 + $T58) <<< 10)}]
      set C [expr {$D + (($C + [I $D $A $B] + $X6  + $T59) <<< 15)}]
      set B [expr {$C + (($B + [I $C $D $A] + $X13 + $T60) <<< 21)}]
      # [ABCD  4  6 61]  [DABC 11 10 62]  [CDAB  2 15 63]  [BCDA  9 21 64]
      set A [expr {$B + (($A + [I $B $C $D] + $X4  + $T61) <<<  6)}]
      set D [expr {$A + (($D + [I $A $B $C] + $X11 + $T62) <<< 10)}]
      set C [expr {$D + (($C + [I $D $A $B] + $X2  + $T63) <<< 15)}]
      set B [expr {$C + (($B + [I $C $D $A] + $X9  + $T64) <<< 21)}]

      # Then perform the following additions. (That is, increment each
      # of the four registers by the value it had before this block
      # was started.)
      incr state(A) $A
      incr state(B) $B
      incr state(C) $C
      incr state(D) $D
    }

    return
  }

  # format 32bits integer in low-to-high order
  proc bytes {v} {
    format %x%x%x%x \
        [expr {0xff & $v}] \
        [expr {0xff & ($v >> 8)}] \
        [expr {0xff & ($v >> 16)}] \
        [expr {0xff & ($v >> 24)}]
  }

  # 32bit rotate-left
  proc <<< {v n} {
    return [expr {((($v << $n) \
                        | (($v >> (32 - $n)) \
                               & (0x7FFFFFFF >> (31 - $n))))) \
                      & 0xFFFFFFFF}]
  }
  # RFC1321:3.4 - function F
  proc F {X Y Z} { return [expr {($X & $Y) | ((~$X) & $Z)}] }
  # RFC1321:3.4 - function G
  proc G {X Y Z} { return [expr {(($X & $Z) | ($Y & (~$Z)))}] }
  # RFC1321:3.4 - function H
  proc H {X Y Z} { return [expr {$X ^ $Y ^ $Z}] }
  # RFC1321:3.4 - function I
  proc I {X Y Z} { return [expr {$Y ^ ($X | (~$Z))}] }

  # Convert our <<< pseudo-operator into a procedure call.
  regsub -all -line \
        {\[expr {(\$[ABCD]) \+ \(\((.*)\)\s+<<<\s+(\d+)\)}\]} \
        $body \
        {[expr {int(\1 + [<<< [expr {\2}] \3])}]} \
        body

  # Inline the F function
  regsub -all -line \
        {\[F (\$[ABCD]) (\$[ABCD]) (\$[ABCD])\]} \
        $body \
        {( (\1 \& \2) | ((~\1) \& \3) )} \
        body

  # Inline the G function
  regsub -all -line \
        {\[G (\$[ABCD]) (\$[ABCD]) (\$[ABCD])\]} \
        $body \
        {(((\1 \& \3) | (\2 \& (~\3))))} \
        body

  # Inline the H function
  regsub -all -line \
        {\[H (\$[ABCD]) (\$[ABCD]) (\$[ABCD])\]} \
        $body \
        {(\1 ^ \2 ^ \3)} \
        body

  # Inline the I function
  regsub -all -line \
        {\[I (\$[ABCD]) (\$[ABCD]) (\$[ABCD])\]} \
        $body \
        {(\2 ^ (\1 | (~\3)))} \
        body

  # RFC 1321:3.4 step 4: inline the set of constant modifiers.
  foreach tName {
    T01 T02 T03 T04 T05 T06 T07 T08 T09 T10
    T11 T12 T13 T14 T15 T16 T17 T18 T19 T20
    T21 T22 T23 T24 T25 T26 T27 T28 T29 T30
    T31 T32 T33 T34 T35 T36 T37 T38 T39 T40
    T41 T42 T43 T44 T45 T46 T47 T48 T49 T50
    T51 T52 T53 T54 T55 T56 T57 T58 T59 T60
    T61 T62 T63 T64
  }  tVal {
    0xd76aa478 0xe8c7b756 0x242070db 0xc1bdceee
    0xf57c0faf 0x4787c62a 0xa8304613 0xfd469501
    0x698098d8 0x8b44f7af 0xffff5bb1 0x895cd7be
    0x6b901122 0xfd987193 0xa679438e 0x49b40821

    0xf61e2562 0xc040b340 0x265e5a51 0xe9b6c7aa
    0xd62f105d 0x2441453  0xd8a1e681 0xe7d3fbc8
    0x21e1cde6 0xc33707d6 0xf4d50d87 0x455a14ed
    0xa9e3e905 0xfcefa3f8 0x676f02d9 0x8d2a4c8a

    0xfffa3942 0x8771f681 0x6d9d6122 0xfde5380c
    0xa4beea44 0x4bdecfa9 0xf6bb4b60 0xbebfbc70
    0x289b7ec6 0xeaa127fa 0xd4ef3085 0x4881d05
    0xd9d4d039 0xe6db99e5 0x1fa27cf8 0xc4ac5665

    0xf4292244 0x432aff97 0xab9423a7 0xfc93a039
    0x655b59c3 0x8f0ccc92 0xffeff47d 0x85845dd1
    0x6fa87e4f 0xfe2ce6e0 0xa3014314 0x4e0811a1
    0xf7537e82 0xbd3af235 0x2ad7d2bb 0xeb86d391
  } {
    lappend map \$$tName $tVal
  }
  set body [string map $map $body]
  unset map tName tVal

  # Define the MD5 hashing procedure with inline functions.
  proc MD5Hash {msg} $body
  unset body
}
