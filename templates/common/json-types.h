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
#                                      Anthony Mallet on Mon Apr  9 2012
#

if {[llength $argv] != 1} { error "expected arguments: types" }
lassign $argv types

lang c
'>

/* C99 standard specifies that format macros must be explicitly requested */
#define __STDC_FORMAT_MACROS

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* --- local data ---------------------------------------------------------- */

#define JSON_MINBUF	128

static int	bufcat(char **buf, char **end, size_t *len, int quoted,
                       const char *data);
static int	bufcat_uint64_t(char **buf, char **end, size_t *len, int neg,
                                uint64_t i) __attribute__ ((unused));

<'foreach t $types {'>
static __inline__ int	json_scan_<"[$t mangle]">(
  <"[$t argument reference]">, const char **);
static __inline__ int	json_print_<"[$t mangle]">(char **, char **, size_t *,
  <"[$t argument value]">);
<'}'>

#define json_skip_whitespace(json) (json) += strspn(json, " \t\n\r")

<'foreach t $types {'>

/* <"[--- [$t mangle] ===================================================]"> */

static __inline__ int
json_scan_<"[$t mangle]">(<"[$t argument reference data]">, const char **json)
{
  int s = 0;
<'
  switch -glob -- [$t kind] {
    typedef - {* member} {'>
  if ((s = json_scan_<"[[$t type] mangle]">(data, json))) return s;
<'
    }
    struct - union - exception {'>
  json_skip_whitespace(*json);
  if (*((*json)++) != '{') return EINVAL;
  json_skip_whitespace(*json);
  while(**json != '}') {
    if (*((*json)++) != '"') return EINVAL;
<'      foreach e [$t members] {'>
    if (!strncmp(*json, "<"[$e name]">\"", 1+<"[string length [$e name]]">)) {
      (*json)+= 1+<"[string length [$e name]]">;
      json_skip_whitespace(*json);
      if (*((*json)++) != ':') return EINVAL;
      if ((s = json_scan_<"[[$e type] mangle]">(
             <"[$e pass reference data->[$e name]]">, json))) return s;
    } else
<'      }'>
      return EINVAL;

    json_skip_whitespace(*json);
    if (**json == ',') {
      (*json)++;
      json_skip_whitespace(*json);
    }
  }
  (*json)++;
<'
    }
    sequence {'>
  long i;

  json_skip_whitespace(*json);
  switch (*((*json)++)) {
    case '[':
      json_skip_whitespace(*json);
      i = 0;
      while(**json != ']') {
<'      if {[catch {$t length} l]} {'>
        if ((s = genom_sequence_reserve(data, i+1))) return s;
<'      } else {'>
        if (i >= <"$l">) return EINVAL;
<'      }'>
        if ((s = json_scan_<"[[$t type] mangle]">(
             <"[[$t type] pass reference data->_buffer\[i\]]">, json)))
          return s;
        data->_length = ++i;
        json_skip_whitespace(*json);
        if (**json == ',') {
          (*json)++;
          json_skip_whitespace(*json);
        }
        i++;
      }
      break;

    case '{':
      json_skip_whitespace(*json);
      while(**json != '}') {
        char *d;

        if (*((*json)++) != '"') return EINVAL;
        i = strtol(*json, &d, 0);
        if (d == *json || *d != '"' || i<0) return EINVAL;
        *json = d+1;
        json_skip_whitespace(*json);
        if (*((*json)++) != ':') return EINVAL;
<'      if {[catch {$t length} l]} {'>
        if (data->_length < (unsigned long)i+1) {
          if ((s = genom_sequence_reserve(data, i+1))) return s;
        }
<'      } else {'>
        if (i >= <"$l">) return EINVAL;
<'      }'>
        if ((s = json_scan_<"[[$t type] mangle]">(
             <"[[$t type] pass reference data->_buffer\[i\]]">, json)))
          return s;
        if (data->_length < (unsigned long)i+1) data->_length = i+1;
        json_skip_whitespace(*json);
        if (**json == ',') {
          (*json)++;
          json_skip_whitespace(*json);
        }
      }
      break;

    default: return EINVAL;
  }
  (*json)++;
<'    }'>
<'    optional {'>
  json_skip_whitespace(*json);
  data->_present = 0;
  if (!strncmp(*json, "null", 4)) {
    (*json) += 4;
    return 0;
  }
  if ((s = json_scan_<"[[$t type] mangle]">(
         <"[[$t type] pass reference data->_value]">, json)))
    return s;
  data->_present = 1;
<'    }'>
<'    array {'>
  long i;

  json_skip_whitespace(*json);
  switch (*((*json)++)) {
    case '[':
      json_skip_whitespace(*json);
      i = 0;
      while(**json != ']') {
        if (i >= <"[$t length]">) return EINVAL;
        if ((s = json_scan_<"[[$t type] mangle]">(
               <"[[$t type] pass reference data\[i\]]">, json))) return s;
        json_skip_whitespace(*json);
        if (**json == ',') {
          (*json)++;
          json_skip_whitespace(*json);
        }
        i++;
      }
      break;

    case '{':
      json_skip_whitespace(*json);
      while(**json != '}') {
        char *d;

        if (*((*json)++) != '"') return EINVAL;
        i = strtol(*json, &d, 0);
        if (d == *json || *d != '"' || i<0) return EINVAL;
        *json = d+1;
        json_skip_whitespace(*json);
        if (*((*json)++) != ':') return EINVAL;
        if (i >= <"[$t length]">) return EINVAL;
        if ((s = json_scan_<"[[$t type] mangle]">(
               <"[[$t type] pass reference data\[i\]]">, json))) return s;
        json_skip_whitespace(*json);
        if (**json == ',') {
          (*json)++;
          json_skip_whitespace(*json);
        }
      }
      break;

    default: return EINVAL;
  }
  (*json)++;
<'    }'>
<'    char {'>
  json_skip_whitespace(*json);
  if (*((*json)++) != '"') return EINVAL;
  *data = *((*json)++);
  if (*data == '\\' && (**json == '"' || **json == '\\'))
    *data = *((*json)++);
  if (*((*json)++) != '"') return EINVAL;
<'    }'>
<'    string {'>
  const char *d;
  char *p;

  json_skip_whitespace(*json);
  if (**json != '"') return EINVAL;
  d = ++(*json);
  while (**json != '"') {
    if (**json == '\\') (*json)++;
    (*json)++;
  }
<'      if {[catch {$t length} l]} {'>
  if (*data) free(*data);
  *data = (char *)malloc(*json - d + 1);
  if (!*data) return ENOMEM;
  strncpy(*data, d, *json - d);
  (*data)[*json - d] = '\0';
  p = *data;
<'      } else {'>
  if (*json - d + 1 > <"$l">) return EINVAL;
  strncpy(data, d, *json - d);
  data[*json - d] = '\0';
  p = data;
<'      }'>
  while((p = strstr(p, "\\"))) {
    if (*p == '"' || *p == '\\')
      memmove(p, p+1, strlen(p)/*including final \0*/);
    p++;
  }
  (*json)++;
<'    }'>
<'    enum {'>
  json_skip_whitespace(*json);
  if (*((*json)++) != '"') return EINVAL;
<'      foreach e [$t members] {'>
  if (!strncmp(*json, "<"[$e fullname]">\"",
               1+<"[string length [$e fullname]]">)) {
    (*json)+= 1+<"[string length [$e fullname]]">;
    *data = <"[$e cname]">;
  } else
    if (!strncmp(*json, "<"[$e name]">\"", 1+<"[string length [$e name]]">)) {
    (*json)+= 1+<"[string length [$e name]]">;
    *data = <"[$e cname]">;
  } else
<'      }'>
    if (!strncmp(*json, "<invalid>\"", sizeof("<invalid>"))) {
    (*json)+= sizeof("<invalid>");
    *data = (<"[$t declarator]">)-1 /*any invalid value*/;
  } else
    return EINVAL;
<'
    }
    float - double {'>
  double v;

  if (!strncmp(*json, "null", 4)) {
    (*json) += 4;
    *data = (<"[$t declarator]">)nan("");
  } else {
    char *d;
    v = strtod(*json, &d);
    if (d == *json) return EINVAL;
    *json = d;
    *data = (<"[$t declarator]">)v;
  }
<'
    }
    default {'>
  long long v;
  char *d;

  v = strtoll(*json, &d, 0);
  if (d == *json) return EINVAL;
  *json = d;
  <"[$t dereference reference data]"> = (<"[$t declarator]">)v;
<'
    }
  }'>
  return s;
}

static __inline__ int
json_print_<"[$t mangle]">(char **json, char **end, size_t *len,
  <"[$t argument value data]">)
{
  int s;
<'
  switch -glob -- [$t kind] {
    typedef - {* member} {'>
  if ((s = json_print_<"[[$t type] mangle]">(json, end, len, data))) return s;
<'
    }
    struct - union - exception {'>
  if ((s = bufcat(json, end, len, 0, "{"))) return s;
<'
      set sep {}
      foreach e [$t members] {'>
  if ((s = bufcat(json, end, len, 0, "<"$sep">\"<"[$e name]">\":"))) return s;
  if ((s = json_print_<"[$e mangle]">(json, end, len,
         <"[$e pass value data->[$e name]]">))) return s;
<'
        set sep ,
      }'>
  if ((s = bufcat(json, end, len, 0, "}"))) return s;
<'
    }
    sequence {'>
  size_t i;
  if ((s = bufcat(json, end, len, 0, "["))) return s;
  for (i=0; i<data->_length; i++) {
    if (i && (s = bufcat(json, end, len, 0, ","))) return s;
    if ((s = json_print_<"[[$t type] mangle]">(json, end, len,
           <"[[$t type] pass value {data->_buffer[i]}]">))) return s;
  }
  if ((s = bufcat(json, end, len, 0, "]"))) return s;
<'    }'>
<'    optional {'>
  if (data->_present) {
    if ((s = json_print_<"[[$t type] mangle]">(json, end, len,
           <"[[$t type] pass value {data->_value}]">))) return s;
  } else {
    if ((s = bufcat(json, end, len, 0, "null"))) return s;
  }
<'    }'>
<'    array {'>
  size_t i;
  if ((s = bufcat(json, end, len, 0, "["))) return s;
  for (i=0; i<<"[$t length]">; i++) {
    if (i && (s = bufcat(json, end, len, 0, ","))) return s;
    if ((s = json_print_<"[[$t type] mangle]">(json, end, len,
           <"[[$t type] pass value {data[i]}]">))) return s;
  }
  if ((s = bufcat(json, end, len, 0, "]"))) return s;
<'    }'>
<'    char {'>
  char buf[] = { '"', *data, '"', 0 };
  if ((s = bufcat(json, end, len, 1, buf))) return s;
<'    }'>
<'    string {'>
  if ((s = bufcat(json, end, len, 0, "\""))) return s;
  if ((s = bufcat(json, end, len, 1, data))) return s;
  if ((s = bufcat(json, end, len, 0, "\""))) return s;
<'    }'>
<'    enum {'>
  switch(data) {
<'      foreach e [$t members] {'>
    case <"[$e cname]">:
      if ((s = bufcat(json, end, len, 0, "\"<"[$e fullname]">\""))) return s;
      break;
<'      }'>
    default:
      assert(!"invalid value in enum <"[$e fullname]">");
      if ((s = bufcat(json, end, len, 0, "\"<invalid>\""))) return s;
      break;
  }
<'
    }
    float - double {'>
  if (isnan(data)) {
    if ((s = bufcat(json, end, len, 0, "null"))) return s;
  } else {
    char buf[32];
    sprintf(buf, "%g", data);
    if ((s = bufcat(json, end, len, 0, buf))) return s;
  }
<'
    }
    {unsigned *} {'>
  uint64_t v = <"[$t dereference value data]">;
  if ((s = bufcat_uint64_t(json, end, len, 0, v))) return s;
<'
    }
    default {'>
  int64_t v = <"[$t dereference value data]">;
  if ((s = bufcat_uint64_t(json, end, len, v<0, (uint64_t)(v<0?-v:v))))
    return s;
<'
    }
  }'>
  return 0;
}
<'}'>

/* --- bufcat -------------------------------------------------------------- */

static int
bufcat(char **buf, char **end, size_t *len, int quoted, const char *data)
{
  size_t avail;
  const char *s;
  char *d;

  s = data;
  while(*s) {
    avail = *len - (*end - *buf);
    if (quoted) {
      for(d = *end; *s && avail; avail--) {
        if (*s == '"') {
          if (avail < 2) {
            avail = 0;
            break;
          }
          *d++ = '\\';
          avail--;
        }
        *d++ = *s++;
      }
    } else {
      for(d = *end; *s && avail; avail--)
        *d++ = *s++;
    }

    if (!avail) {
      char *newbuf = (char *)realloc(*buf, *len << 1);
      if (!*newbuf) return ENOMEM;

      d = newbuf + (d - *buf);
      *buf = newbuf;
      *len <<= 1;
    }
    *d = 0;
    *end = d;
  }

  return 0;
}

static int
bufcat_uint64_t(char **buf, char **end, size_t *len, int neg, uint64_t i)
{
  char a[32];
  char *d;

  d = a + sizeof(a);
  *--d = 0;
  do { *--d = '0' + i%10; } while (i /= 10);
  if (neg) *--d = '-';

  return bufcat(buf, end, len, 0, d);
}
