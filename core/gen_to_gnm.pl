#!/usr/bin/env perl

use strict;
use File::Basename;

# $line = "   requires: \"pom >= 0.2\"\n";

my %tags = ("module", "component",
	"request", "service",
	"exec_task", "task"
	);

my @unused = ("posters_input", "requires", "codel_requires");

my %fields = (
# component
  "number", "uniqueId",
  "internal_data", "ids",
# service
  "exec_task", "taskName",
  "fail_reports", "errors",
  "interrupt_activity", "interrupts"
	  );

my %codels= ("c_init_func", "init",
	  "codel_task_start", "init",
	  "codel_task_end", "end"
	  );

my $input = "/home/ccpasteur/work/tuto/demo.gen";
open(IN, $input);
my @lines = <IN>;

my $filename, my $directories, my $suffix;
($filename, $directories, $suffix) = fileparse($input, qr/\.[^.]*/);
my $output = $directories.$filename.".gnm";
print "Writing to $output\n";
open(OUT, ">$output");

my $line = "";
foreach $line (@lines) {
#   print "Examining: $line";

  if($line =~ /^[\s\t]*([a-zA-Z_]*)\s([a-zA-Z\d_]*)\s*{/) {
#     print "Matched tag $1";
    if($tags{$1} eq "") {
      print OUT $line;
    } else {
      print OUT "$tags{$1} $2 {\n";
    }
  } elsif($line =~ /^[\s\t]*codel_([a-zA-Z_]*)[\s\t]*:[\s\t]*([a-zA-Z\d_]*);/) {
#     print "Found codel declaration: $1";
    print OUT "codel $1: $2();\n";
  } elsif($line =~ /^[\s\t]*(c_init_func|codel_task_start)[\s\t]*:[\s\t]*([a-zA-Z\d_]*);/) {
#     print "Found task codel declaration: $1";
    if($codels{$1} eq "") {
      print OUT $line;
    } else {
      print OUT "codel $codels{$1}: $2();\n";
    }
  } elsif($line =~ /^[\s\t]*(input|output)[\s\t]*:[\s\t]*([a-zA-Z\d_]*)::([a-zA-Z\d_]*)[\s\t]*;/) {
#     print "Found $1";
    print OUT "$1: IDS.$2;\n";
  } elsif($line =~ /^[\s\t]*([a-zA-Z_]*)[\s\t]*:(.*)/) {
#     print "Matched field $1";
    if(grep { $_ eq $1 } @unused) {
      print OUT "// unsupported $line"
    } elsif($fields{$1} eq "") {
      print OUT $line;
    } else {
      print OUT "$fields{$1}: $2\n";
    }
  } else {
#     print "No match";
      print OUT $line;
  }

}
