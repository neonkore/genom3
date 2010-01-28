/*
 * Copyright (c) 2009-2010 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and binary  forms,  with  or  without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 * THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
 * WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
 * MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
 * ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
 * WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *                                           Anthony Mallet on Sun Dec 20 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

/** hash table entries */
struct hentry_s {
  const char *key;		/**< hash key */
  void *value;			/**< user data */
  hrelease_f release;		/**< function to release user data */

  struct hentry_s *succ;	/**< next entry in order of insertion */
  struct hentry_s *pred;	/**< previous entry in order of insertion */

  struct hentry_s *next;	/**< next entry in a bucket */
};

/** hash table */
struct hash_s {
  const char *name;	/**< a name, for verbose messages */
  unsigned int n;	/**< number of entries */

  hentry_s first;	/**< first entry in order of insertion */
  hentry_s last;	/**< last entry in order of insertion */

  unsigned int buckets;	/**< number of buckets */
  hentry_s *bucket;	/**< buckets, array of entries */
};

/** expand hash table beyond this number of entries per bucket on average */
#define hash_maxload	(2.)


static int		hash_expand(hash_s h);
static unsigned long	hstring(const char *key);


/* --- hash_create --------------------------------------------------------- */

/** Create a hash table with the number of expected entries
 */

hash_s
hash_create(const char *name, int entries)
{
  hash_s h;

  h = malloc(sizeof(*h));
  if (!h) return NULL;

  h->name = name;
  h->n = 0;
  h->first = NULL;
  h->last = NULL;

  h->buckets = (int)(entries/hash_maxload)/2*2;
  if (h->buckets < 1) h->buckets = 1;
  if (h->buckets > 1024) h->buckets = 1024;

  h->bucket = calloc(h->buckets, sizeof(*h->bucket));
  if (!h->bucket) { free(h); return NULL; }

  return h;
}


/* --- hash_destroy -------------------------------------------------------- */

/** Delete a hash table and all its entries
 */

void
hash_destroy(hash_s h)
{
  int i;
  hentry_s e;

  if (!h) return;

  for(i=0; i<h->buckets; i++)
    while(h->bucket[i]) {
      e = h->bucket[i];
      h->bucket[i] = e->next;

      free((char *)e->key);
      if (e->release) e->release(e->value);
      free(e);
    }

  free(h->bucket);
  free(h);
}


/* --- hash_insert --------------------------------------------------------- */

/** Create a new hash table entry
 */

int
hash_insert(hash_s h, const char *key, void *value, hrelease_f release)
{
  unsigned long index;
  hentry_s e;

  assert(h); assert(key);

  /* prevent duplicates */
  index = hstring(key) % h->buckets;
  for(e = h->bucket[index]; e; e = e->next)
    if (!strcmp(e->key, key)) {
      xwarnx("attempt to reinsert existing `%s' in %s hash", key, h->name);
      return errno = EEXIST;
    }

  /* create entry */
  e = malloc(sizeof(*e));
  if (!e) return ENOMEM;
  e->key = strdup(key);
  e->value = value;
  e->release = release;
  if (!e->key) {
    free(e);
    warnx("memory exhausted, cannot insert `%s' in %s hash", key, h->name);
    return errno = ENOMEM;
  }

  /* preserve insertion order information */
  e->pred = h->last;
  e->succ = NULL;
  if (!h->first) h->first = e;
  if (!h->last) h->last = e; else { h->last->succ = e; h->last = e; }

  /* insert into proper bucket, resizing the hash if needed */
  if (h->n > h->buckets*hash_maxload) {
    hash_expand(h);
    index = hstring(key) % h->buckets;
  }

  e->next = h->bucket[index];
  h->bucket[index] = e;
  h->n++;

  return 0;
}


/* --- hash_set ------------------------------------------------------------ */

/** Update the value of an entry in hash table
 */
int
hash_set(hash_s h, const char *key, void *value)
{
  unsigned long index;
  hentry_s e;

  assert(h); assert(key);

  index = hstring(key) % h->buckets;
  for(e = h->bucket[index]; e; e = e->next)
    if (!strcmp(e->key, key)) {
      e->value = value;
      return 0;
    }

  return ENOENT;
}


/* --- hash_rename --------------------------------------------------------- */

/** Rename the key of a hash table entry. Don't use remove/insert to preserve
 * insertion order.
 */
int
hash_rename(hash_s h, const char *key, const char *new)
{
  unsigned long index;
  hentry_s e, n;

  assert(h && key && new);

  /* unlink entry */
  index = hstring(key) % h->buckets;
  e = NULL;
  if (h->bucket[index]) {
    if (!strcmp(h->bucket[index]->key, key)) {
      /* entry is in first bucket */
      e = h->bucket[index];
      h->bucket[index] = e->next;
    } else
      for(n = h->bucket[index]; n->next; n = n->next)
	if (!strcmp(n->next->key, key)) {
	  /* entry is in random bucket */
	  e = n->next;
	  n->next = e->next;
	  break;
	}
  }
  if (!e) return ENOENT;

  /* assign new key */
  free((char *)e->key); e->key = strdup(new);

  /* insert into new bucket */
  index = hstring(new) % h->buckets;
  e->next = h->bucket[index];
  h->bucket[index] = e;

  return 0;
}


/* --- hash_remove --------------------------------------------------------- */

/** Remove an entry from hash table
 */

int
hash_remove(hash_s h, const char *key, int release)
{
  unsigned long index;
  hentry_s e, n;

  assert(h); assert(key);

  /* look for entry */
  index = hstring(key) % h->buckets;
  e = NULL;
  if (h->bucket[index]) {
    if (!strcmp(h->bucket[index]->key, key)) {
      /* entry is in first bucket */
      e = h->bucket[index];
      h->bucket[index] = e->next;
    } else
      for(n = h->bucket[index]; n->next; n = n->next)
	if (!strcmp(n->next->key, key)) {
	  /* entry is in random bucket */
	  e = n->next;
	  n->next = e->next;
	  break;
	}
  }
  if (!e) return ENOENT;

  /* preserve insertion order */
  if (h->first == e) h->first = e->succ;
  if (h->last == e) h->last = e->pred;
  if (e->pred) e->pred->succ = e->succ;
  if (e->succ) e->succ->pred = e->pred;

  /* release entry */
  free((char *)e->key);
  if (release && e->release) e->release(e->value);
  free(e);
  h->n--;

  return 0;
}


/* --- hash_find ----------------------------------------------------------- */

/** Return an entry in hash table
 */

void *
hash_find(hash_s h, const char *key)
{
  unsigned long index;
  hentry_s e;

  assert(h); assert(key);

  index = hstring(key) % h->buckets;
  for(e = h->bucket[index]; e; e = e->next)
    if (!strcmp(e->key, key))
      return e->value;

  return NULL;
}


/* --- hash_first/next ----------------------------------------------------- */

/** Return the first/next entry in hash table
 */

int
hash_first(hash_s h, hiter *i)
{
  assert(h); assert(i);

  i->current = h->first;
  i->key = h->first?h->first->key:NULL;
  i->value = h->first?h->first->value:NULL;

  return h->first?0:ENOENT;
}

int
hash_after(hash_s h, void *value, hiter *i)
{
  assert(i);

  for(hash_first(h, i); i->current; hash_next(i)) {
    if (i->value == value) return hash_next(i);
  }

  return ENOENT;
}

int
hash_next(hiter *i)
{
  assert(i);

  i->current = i->current?i->current->succ:NULL;
  i->key = i->current?i->current->key:NULL;
  i->value = i->current?i->current->value:NULL;

  return i->current?0:ENOENT;
}


/* --- hash_pstat ---------------------------------------------------------- */

/** Print statistics regarding the number of entries per bucket
 */

void
hash_pstat(hash_s h)
{
#define hslots (6)
  int count[hslots] = { 0 };
  hentry_s e;
  int i, c;

  /* histogram of bucket usage */
  for(i=0; i<h->buckets; i++) {
    for(c = 0, e = h->bucket[i]; e; c++, e = e->next) /* empty body */;
    count[(c>=hslots)?hslots-1:c]++;
  }

  /* output info */
  warnx("statistics for %s hash", h->name);
  warnx(" %d entries in total, %d buckets (%d empty)",
	h->n, h->buckets, count[0]);
  for(i=1; i<hslots-1; i++)
    warnx(" buckets with %d entr%s: %d", i, i>1?"ies":"y", count[i]);
  warnx(" buckets with %d entries or more: %d", hslots-1, count[hslots-1]);
}


/* --- hash_expand --------------------------------------------------------- */

/** Rebuild a bigger hash table
 */

static int
hash_expand(hash_s h)
{
  hentry_s *oldb = h->bucket;
  unsigned int oldn = h->buckets;
  unsigned int n = h->buckets * 2;
  unsigned int index;
  hentry_s p, e;
  int i;

  h->bucket = calloc(n, sizeof(*h->bucket));
  if (!h->bucket) { h->bucket = oldb; return ENOMEM; }
  h->buckets = n;

  for(i=0; i<oldn; i++)
    for(p = oldb[i]; p;) {
      e = p;
      p = p->next;

      index = hstring(e->key) % h->buckets;
      e->next = h->bucket[index];
      h->bucket[index] = e;
    }

  free(oldb);
  xwarnx("expanded hash %s with %d entries to %d buckets (was %d)",
	 h->name, h->n, h->buckets, oldn);
  return 0;
}


/* --- hstring ------------------------------------------------------------- */

/** A supposedly good hash function for strings.
 */
static unsigned long
hstring(const char *key)
{
  unsigned long hash = 0;
  if (!key) return 0;

  while(*key) {
    hash += (hash << 3) + (unsigned char)*(key++);
  }
  return hash;
}
