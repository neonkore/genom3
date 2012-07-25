/*
 * Copyright (c) 2012 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 *					Anthony Mallet on Sun Mar 25 2012
 */

#ifndef H_GENOM3_C_CLIENT
#define H_GENOM3_C_CLIENT

#include <stddef.h>
#include <stdint.h>

#define GENOM_CLIENT_DLSYM		"genom_client_info"

enum { genom_client_protocol = 20120725 };

typedef struct genom_client_s *genom_client;

typedef void (*genom_request_cb)(
  genom_client h, int rqstid, int error, uint32_t report, void *output,
  void *cb_data);
typedef int (*genom_request_sendfn)(
  genom_client h, const void *in,
  genom_request_cb sentcb, genom_request_cb donecb, void *cb_data,
  int *rqstid);

typedef int (*genom_port_readfn)(genom_client h, void *data);
typedef int (*genom_port_set_readfn)(genom_client h,
  const char *name, void *data);

typedef void (*genom_initfn)(void *data);
typedef void (*genom_finifn)(void *data);

typedef int (*genom_json_scanfn)(void *in, const char *json, char **endptr);
typedef int (*genom_json_printfn)(char **json, void *out);

struct genom_service_info {
  const char *name;
  const char *input;
  const char *output;
  const char *meta;
  genom_initfn init_input, init_output;
  genom_finifn fini_input, fini_output;
  genom_json_scanfn json_scan;
  genom_json_printfn json_print;
  genom_request_sendfn send;
  size_t input_size;
  size_t output_size;
};

struct genom_port_info {
  const char *name;
  const char *data;
  const char *meta;
  genom_initfn init_data;
  genom_finifn fini_data;
  genom_json_printfn json_print;
  unsigned int regular;
  union {
    genom_port_readfn regular;
    genom_port_set_readfn set;
  } read;
  size_t data_size;
};

struct genom_client_info {
  const uint32_t protocol;
  const char *name;

  genom_client (*init)(int argc, char *argv[]);
  void (*fini)(genom_client h);
  const char *(*instance)(genom_client h);
  int (*eventfd)(genom_client h);
  const struct genom_service_info *(*service_info)(genom_client h, int rqstid);
  int (*done)(genom_client h, int rqstid);
  int (*wait)(genom_client h, int rqstid);
  int (*clean)(genom_client h, int rqstid);
  int (*doevents)(genom_client h);
  const char *(*strerror)(genom_client h);

  int nservices;
  const struct genom_service_info *services;

  int nports;
  const struct genom_port_info *ports;
};

#endif /* H_GENOM3_C_CLIENT */
