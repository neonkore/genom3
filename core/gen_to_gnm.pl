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

if($#ARGV == 1) {
  print "Usage: gen_to_gnm.pl path/to/gen_file.gen.\n";
  exit(0);
}

my $input = $ARGV[0];
print "Reading from file: $input\n";
open(IN, $input);
my @lines = <IN>;

my $filename, my $directories, my $suffix;
($filename, $directories, $suffix) = fileparse($input, qr/\.[^.]*/);
my $output = $directories.$filename.".gnm";
print "Writing to $output\n";
open(OUT, ">$output");

my $line = "";
for(my $i = 0; $i < scalar(@lines); $i++) {
#   print "Examining: $line";
  $line = $lines[$i];

  if($line =~ /^([\s\t]*)([a-zA-Z_]*)\s([a-zA-Z\d_]*)\s*{/) {
#     print "Matched tag $1";
    if($2 eq "poster") {
      print OUT "// syntax: Outport Type Name;\n";
      while(not $line =~ /.*}[\s\t]*;/) {
	print OUT "// $line";
	$i++;
	$line = $lines[$i];
      }
      print OUT "// $line"
    } elsif($tags{$2} eq "") {
      print OUT $line;
    } else {
      print OUT "$1$tags{$2} $3 {\n";
    }
  } elsif($line =~ /^([\s\t]*)codel_([a-zA-Z_]*)[\s\t]*:[\s\t]*([a-zA-Z\d_]*);/) {
#     print "Found codel declaration: $1";
    print OUT "$1codel $2: $3();\n";
  } elsif($line =~ /^([\s\t]*)(c_init_func|codel_task_start)[\s\t]*:[\s\t]*([a-zA-Z\d_]*);/) {
#     print "Found task codel declaration: $1";
    if($codels{$2} eq "") {
      print OUT $line;
    } else {
      print OUT "$1codel $codels{$2}: $3();\n";
    }
  } elsif($line =~ /^([\s\t]*)(input|output)[\s\t]*:[\s\t]*([a-zA-Z\d_]*)::([a-zA-Z\d_]*)[\s\t]*;/) {
#     print "Found $1";
    print OUT "$1$2: IDS.$3;\n";
  } elsif($line =~ /^([\s\t]*)([a-zA-Z_]*)[\s\t]*:(.*)/) {
#     print "Matched field $1";
    if(grep { $_ eq $2 } @unused) {
      print OUT "// unsupported $line"
    } elsif($fields{$2} eq "") {
      print OUT $line;
    } else {
      print OUT "$1$fields{$2}: $3\n";
    }
  } else {
#     print "No match";
      print OUT $line;
  }

}
