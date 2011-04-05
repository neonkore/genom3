# Copyright (c) 2003 by Kevin B. Kenny. All rights reserved.
#
# The authors hereby  grant permission to use, copy,  modify, distribute, and
# license this software and its  documentation for any purpose, provided that
# existing copyright notices are retained  in all copies and that this notice
# is included  verbatim in any distributions. No  written agreement, license,
# or royalty fee  is required for any of  the authorized uses.  Modifications
# to this  software may be copyrighted  by their authors and  need not follow
# the licensing terms described here, provided that the new terms are clearly
# indicated on the first page of each file where they apply.
#
# IN NO  EVENT SHALL THE AUTHORS OR  DISTRIBUTORS BE LIABLE TO  ANY PARTY FOR
# DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
# OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
# EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# THE  AUTHORS   AND  DISTRIBUTORS  SPECIFICALLY   DISCLAIM  ANY  WARRANTIES,
# INCLUDING, BUT  NOT LIMITED TO, THE IMPLIED  WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR  A PARTICULAR PURPOSE,  AND NON-INFRINGEMENT. THIS  SOFTWARE IS
# PROVIDED ON  AN "AS  IS" BASIS,  AND THE AUTHORS  AND DISTRIBUTORS  HAVE NO
# OBLIGATION  TO  PROVIDE  MAINTENANCE,  SUPPORT, UPDATES,  ENHANCEMENTS,  OR
# MODIFICATIONS.
#
# GOVERNMENT  USE:  If you  are  acquiring this  software  on  behalf of  the
# U.S. government, the Government shall  have only "Restricted Rights" in the
# software and  related documentation as  defined in the  Federal Acquisition
# Regulations (FARs)  in Clause 52.227.19 (c)  (2). If you  are acquiring the
# software  on behalf of  the Department  of Defense,  the software  shall be
# classified as "Commercial Computer  Software" and the Government shall have
# only  "Restricted Rights"  as defined  in  Clause 252.227-7013  (c) (1)  of
# DFARs. Notwithstanding the foregoing, the authors grant the U.S. Government
# and  others acting  in  its behalf  permission  to use  and distribute  the
# software in accordance with the terms specified in this license.

namespace eval merge {

  # -- lcs -----------------------------------------------------------------
  #
  # Computes the longest common subsequence of two files.
  #
  # This procedure is a slight variation of the
  # ::struct::list::LlongestCommonSubsequence procedure as found in the tcllib
  # project: http://tcllib.sourceforge.net/. Blank lines are ignored.
  #
  # Parameters:
  #	sequence1, sequence2	Two lists to compare.
  #	maxOccurs		If provided, causes the procedure to ignore
  #				lines that appear more than $maxOccurs times in
  #				the second file. See below for a discussion.
  # Results:
  #	Returns a list of two lists of equal length.
  #	The first sublist is of indices into file1, and the second sublist is
  #	of indices into file2. Each corresponding pair of indices corresponds
  #	to equal elements in the files; the sequence returned is the longest
  #	possible with blank lignes ignored.
  #
  # Notes:
  #
  # While this procedure is quite rapid for many tasks of file comparison, its
  # performance degrades severely if the second file contains many equal
  # elements (as, for instance, when using this procedure to compare two files,
  # a quarter of whose lines are blank). This drawback is intrinsic to the
  # algorithm used (see the References for details). One approach to dealing
  # with this problem that is sometimes effective in practice is arbitrarily to
  # exclude elements that appear more than a certain number of times. This
  # number is provided as the 'maxOccurs' parameter. If frequent lines are
  # excluded in this manner, they will not appear in the common subsequence
  # that is computed; the result will be the longest common subsequence of
  # infrequent elements.
  #
  # References:
  # J. W. Hunt and M. D. McIlroy, "An algorithm for differential file
  # comparison," Comp. Sci. Tech. Rep. #41, Bell Telephone Laboratories
  # (1976). Available on the Web at the second author's personal site:
  # http://www.cs.dartmouth.edu/~doug/
  #
  proc lcs { sequence1 sequence2 {maxOccurs 0x7fffffff} } {

    # Construct a set of equivalence classes of lines in file 2
    set index 0
    foreach string $sequence2 {
      if {[string trim $string] != ""} {
        lappend eqv($string) $index
      }
      incr index
    }

    # K holds descriptions of the common subsequences. Initially, there is one
    # common subsequence of length 0, with a fence saying that it includes line
    # -1 of both files. The maximum subsequence length is 0; position 0 of K
    # holds a fence carrying the line following the end of both files.

    lappend K [::list -1 -1 {}]
    lappend K [::list [llength $sequence1] [llength $sequence2]]
    set k 0

    # Walk through the first file, letting i be the index of the line and
    # string be the line itself.

    set i 0
    foreach string $sequence1 {
      # Consider each possible corresponding index j in the second file.

      if { [info exists eqv($string)]
           && [llength $eqv($string)] <= $maxOccurs } {

        # c is the candidate match most recently found, and r is the length of
        # the corresponding subsequence.

        set r 0
        set c [lindex $K 0]

        foreach j $eqv($string) {
          # Perform a binary search to find a candidate common subsequence to
          # which may be appended this match.

          set max $k
          set min $r
          set s [expr { $k + 1 }]
          while { $max >= $min } {
            set mid [expr { ( $max + $min ) / 2 }]
            set bmid [lindex [lindex $K $mid] 1]
            if { $j == $bmid } {
              break
            } elseif { $j < $bmid } {
              set max [expr {$mid - 1}]
            } else {
              set s $mid
              set min [expr { $mid + 1 }]
            }
          }

          # Go to the next match point if there is no suitable candidate.

          if { $j == [lindex [lindex $K $mid] 1] || $s > $k} {
            continue
          }

          # s is the sequence length of the longest sequence to which this
          # match point may be appended. Make a new candidate match and store
          # the old one in K. Set r to the length of the new candidate match.

          set newc [::list $i $j [lindex $K $s]]
          if { $r >= 0 } {
            lset K $r $c
          }
          set c $newc
          set r [expr { $s + 1 }]

          # If we've extended the length of the longest match, we're done; move
          # the fence.

          if { $s >= $k } {
            lappend K [lindex $K end]
            incr k
            break
          }
        }

        # Put the last candidate into the array

        lset K $r $c
      }

      incr i
    }

    # Package the common subsequence in a convenient form

    set seta {}
    set setb {}
    set q [lindex $K $k]

    for { set i 0 } { $i < $k } {incr i } {
      lappend seta {}
      lappend setb {}
    }
    lappend seta [llength $sequence1]
    lappend setb [llength $sequence2]
    while { [lindex $q 0] >= 0 } {
      incr k -1
      lset seta $k [lindex $q 0]
      lset setb $k [lindex $q 1]
      set q [lindex $q 2]
    }

    return [::list $seta $setb]
  }
}
