/*
 *  linkedlist.h
 *  
 *
 *  Created by Vincent Gramoli on 1/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "harris.h"

int set_contains(intset_t *set, key_type val, int transactional);
int set_add(intset_t *set, key_type key, val_type val, int transactional);
int set_remove(intset_t *set, key_type val, int transactional);

