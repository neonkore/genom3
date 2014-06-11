<'
# Copyright (c) 2012-2014 LAAS/CNRS
# All rights reserved.
#
# Redistribution and use  in source  and binary  forms,  with or without
# modification, are permitted provided that the following conditions are
# met:
#
#   1. Redistributions of  source  code must retain the  above copyright
#      notice and this list of conditions.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice and  this list of  conditions in the  documentation and/or
#      other materials provided with the distribution.
#
#                                      Anthony Mallet on Tue Mar 13 2012
#

if {[llength $argv] != 2} { error "expected arguments: component types" }
lassign $argv component types

# compute handy shortcuts
set comp [$component name]
set COMP [string toupper [$component name]]

lang c
'>
#ifndef H_<"$COMP">_CLIENT
#define H_<"$COMP">_CLIENT

<'
set hlist [list]
foreach s [$component services] {
  foreach p [$s parameters] {
    switch -- [[$p type] kind] {
      boolean { lappend hlist stdbool.h }
    }
  }
}
foreach h [lsort -unique $hlist] {'>
#include <<"$h">>
<'}'>

#include "genom3/c/client.h"


/* --- Interface types ----------------------------------------------------- */

<'foreach t $types {'>
<"[language mapping $t]">
<'}'>


/* --- Service input and output structures --------------------------------- */

<'foreach s [$component services] {'>

/* input of <"[$s name]"> */
struct genom_<"$comp">_<"[$s name]">_input {
<'  foreach p [$s parameters in inout] {'>
#line <"[lindex [$p loc] 1]"> "<"[lindex [$p loc] 0]">"
  <"[[$p type] declarator [$p name]]">;
<'  }'>
};

/* output of <"[$s name]"> */
struct genom_<"$comp">_<"[$s name]">_output {
<'  foreach p [$s parameters out inout] {'>
#line <"[lindex [$p loc] 1]"> "<"[lindex [$p loc] 0]">"
  <"[[$p type] declarator [$p name]]">;
<'  }'>
};

<'  }'>


#ifdef __cplusplus
extern "C" {
#endif

/* --- Services ------------------------------------------------------------ */

genom_client		genom_<"$comp">_client_init(int argc, char *argv[],
                                genom_event *ex, const void **exdetail);
void			genom_<"$comp">_client_fini(genom_client h);
genom_context		genom_<"$comp">_client_context(genom_client h);
const char *		genom_<"$comp">_client_instance(genom_client h);
int			genom_<"$comp">_client_eventfd(genom_client h);
const struct genom_service_info *
			genom_<"$comp">_client_service_info(genom_client h,
				int rqstid);
int			genom_<"$comp">_client_done(genom_client h, int rqstid);
genom_event		genom_<"$comp">_client_wait(genom_client h, int rqstid);
genom_event		genom_<"$comp">_client_clean(genom_client h,
				int rqstid);
genom_event		genom_<"$comp">_client_abort(genom_client h,
                                int rqstid);
genom_event		genom_<"$comp">_client_doevents(genom_client h);
char *			genom_<"$comp">_client_json_error(genom_client h,
				genom_event e, const void *detail);

<'foreach s [$component services] {'>
/* <"[$s name]"> */
typedef void (*<"$comp">_<"[$s name]">_cb)(
  genom_client h,
  int rqstid,
  genom_event report,
  struct genom_<"$comp">_<"[$s name]">_output *out,
  const void *exdetail,
  void *cb_data);

genom_event genom_<"$comp">_client_<"[$s name]">_rqst(
  genom_client h,
  const struct genom_<"$comp">_<"[$s name]">_input *in,
  <"$comp">_<"[$s name]">_cb sentcb, <"$comp">_<"[$s name]">_cb donecb,
  void *cb_data, int *rqstid);

genom_event genom_<"$comp">_client_<"[$s name]">_result(
  genom_client h, int rqstid, genom_event *report,
  struct genom_<"$comp">_<"[$s name]">_output **out,
  void **exdetail);

void	genom_<"$comp">_client_<"[$s name]">_init_input(
  struct genom_<"$comp">_<"[$s name]">_input *input);
void	genom_<"$comp">_client_<"[$s name]">_init_output(
  struct genom_<"$comp">_<"[$s name]">_output *output);
void	genom_<"$comp">_client_<"[$s name]">_fini_input(
  struct genom_<"$comp">_<"[$s name]">_input *input);
void	genom_<"$comp">_client_<"[$s name]">_fini_output(
  struct genom_<"$comp">_<"[$s name]">_output *output);

int	genom_<"$comp">_client_<"[$s name]">_json_scan(
  struct genom_<"$comp">_<"[$s name]">_input *in,
  const char *json, char **endptr);
int	genom_<"$comp">_client_<"[$s name]">_json_print(
  char **json, const struct genom_<"$comp">_<"[$s name]">_output *out);

<'}'>


/* --- Ports --------------------------------------------------------------- */

<'foreach p [$component ports out] {'>
/* <"[$p name]"> */
#line <"[lindex [$p loc] 1]"> "<"[lindex [$p loc] 0]">"
genom_event genom_<"$comp">_client_<"[$p name]">_port(
  genom_client h,
<'  if {"multiple" in [$p kind]} {'>
  const char *name,
<'  }'>
  <"[[$p datatype] argument reference data]">);

<'}'>

<'foreach p [$component ports out] {'>
void	genom_<"$comp">_client_<"[$p name]">_init_data(
  <"[[$p datatype] argument reference data]">);
void	genom_<"$comp">_client_<"[$p name]">_fini_data(
  <"[[$p datatype] argument reference data]">);
int	genom_<"$comp">_client_<"[$p name]">_json_print(char **json,
  <"[[$p datatype] argument value data]">);

<'}'>



/* --- Data ---------------------------------------------------------------- */

extern const struct genom_client_info genom_<"$comp">_client_info;

<'foreach s [$component services] {'>
extern const struct genom_service_info *
			genom_<"$comp">_client_<"[$s name]">_info;
<'}'>

<'foreach p [$component ports out] {'>
extern const struct genom_port_info *
			genom_<"$comp">_client_<"[$p name]">_info;
<'}'>

#ifdef __cplusplus
}
#endif

#endif /* H_<"$COMP">_CLIENT */
