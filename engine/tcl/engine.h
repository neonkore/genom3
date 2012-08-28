/*
 * Copyright (c) 2010,2012 LAAS/CNRS
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
 *
 *					Anthony Mallet on Fri Jan 22 2010
 */

#ifndef H_ENGINE
#define H_ENGINE

#include <tcl.h>

#include "genom.h"

int	dg_genom_program(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_genom_cmdline(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_genom_version(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_genom_templates(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_genom_debug(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_genom_verbose(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_template_name(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_template_dir(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_template_builtindir(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_template_tmpdir(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_input_file(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_input_base(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_input_dir(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_input_notice(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_input_deps(ClientData v, Tcl_Interp *interp, int objc,
                Tcl_Obj *const objv[]);
int	dg_parse(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_types(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	dg_components(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);

int	type_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	comp_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	task_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	port_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	service_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	remote_cmd(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	codel_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	param_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);
int	initer_cmd(ClientData c, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[]);

char *	type_genref(idltype_s t);
char *	comp_genref(comp_s c);
char *	task_genref(task_s t);
char *	port_genref(port_s p);
char *	service_genref(service_s s);
char *	remote_genref(remote_s r);
char *	codel_genref(codel_s c);
char *	param_genref(param_s p);
char *	initer_genref(initer_s p);

int	engine_createcmd(Tcl_Interp *interp, Tcl_Interp *slave,
		const char *cmd, Tcl_ObjCmdProc *proc, ClientData data);
int	engine_export(Tcl_Interp *interp, Tcl_Interp *slave);

#endif /* H_ENGINE */
