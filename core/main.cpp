/*
 * Copyright (c) 2009 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include <string>
#include <iostream>
#include "libgen.h"

#include "parsers/template_info/templateinterpreter.h"
#include "parsers/genom/driver.h"

using namespace std;
using namespace G3nom;

int main(int argc, char* argv[])
{
	string usage_str(
		"Usage:\n"
		"  genom3 [-t template] [-o out_dir] file\n"
		"with\n"
		"  -h Display this help message\n"
		"  -d Display debug information\n"
		"  -s Do not generate anything, only check the .gnm syntax");

	string templatesDir("/home/ccpasteur/work/git/g3nom/templates/");
	string templ("genom_legacy");

	string outputDir, args;
	bool checkSyntaxMode = false;

	int idx = 1;
	while ((idx < argc) && (argv[idx][0]=='-')) {
		string sw = argv[idx];
		if(sw == "-o") {
			outputDir = argv[++idx];
		} else if(sw == "-t") {
			templ = argv[++idx];
		} else if(sw == "-s") {
			checkSyntaxMode = true;
		} else if(sw == "-u" || sw == "--help" || sw == "-h") {
			cout << usage_str << endl;
			exit(0);
		} else {
			args.append(" ").append(sw.substr(1));
		}
		idx++;
	}

	if(idx >= argc) { // not enough args
		cout << "Not enough arguments" << endl;
		cout << usage_str << endl;
		exit(1);
	}

	TemplateInterpreter ti(args);
	ti.setPrintGeneratedFile(false);
	string sourceDir = templatesDir + templ + "/";

	Driver d;
	if (!d.parseFile(argv[idx])) {
		cout << "Error parsing .gnm file: " << argv[idx] << endl;
		exit(1);
	}
	if(checkSyntaxMode) {
		cout << "File syntax is correct." << endl;
		exit(0);
	}

	ti.setComponent(&(d.component()));

	if(outputDir.empty())
		outputDir = dirname(argv[idx]) + string("/");

	ti.setSourceDirectory(sourceDir);
	ti.setOutputDirectory(outputDir);
	
	ti.parseInfoFile(sourceDir + "template.info");
	return 0;
}
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
