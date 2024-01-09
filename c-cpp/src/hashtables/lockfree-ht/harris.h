/*
 * File:
 *   harris.h
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 * Description:
 *   Lock-free linkedlist implementation of Harris' algorithm
 *   "A Pragmatic Implementation of Non-Blocking Linked Lists" 
 *   T. Harris, p. 300-314, DISC 2001.
 *
 * Copyright (c) 2009-2010.
 *
 * harris.h is part of Synchrobench
 * 
 * Synchrobench is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include "linkedlist.h"

/* ################################################################### *
 * HARRIS' LINKED LIST
 * ################################################################### */

int is_marked_ref(long i);
long unset_mark(long i);
long set_mark(long i);
long get_unmarked_ref(long w);
long get_marked_ref(long w);

node_t *harris_search(intset_t *set, key_type key, node_t **left_node);
int harris_find(intset_t *set, key_type key);
int harris_insert(intset_t *set, key_type key, val_type value);
int harris_delete(intset_t *set, key_type key);
