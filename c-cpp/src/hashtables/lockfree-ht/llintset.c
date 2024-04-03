/*
 *  intset.c
 *  
 *  Integer set operations (contain, insert, delete) 
 *  that call stm-based / lock-free counterparts.
 *
 *  Created by Vincent Gramoli on 1/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "llintset.h"

int set_contains(intset_t *set, key_type key, int transactional)
{
	int result;
	
#ifdef DEBUG
	printf("++> set_contains(%d)\n", (int)key);
	IO_FLUSH;
#endif
	
#ifdef SEQUENTIAL
	node_t *prev, *next;
	
	prev = set->head;
	next = prev->next;
	while (next->key < key) {
		prev = next;
		next = prev->next;
	}
	result = (next->key == key);

#elif defined STM			

	node_t *prev, *next;
	key_type v = 0;

	if (transactional > 1) {

	  TX_START(EL);
	  prev = set->head;
	  next = (node_t *)TX_LOAD(&prev->next);
	  while (1) {
	    v = TX_LOAD((key_type *) &next->key);
	    if (v >= key)
	      break;
	    prev = next;
	    next = (node_t *)TX_LOAD(&prev->next);
	  }		  
	  TX_END;
	  result = (v == key);
	  
	} else {

	  TX_START(NL);
	  prev = set->head;
	  next = (node_t *)TX_LOAD(&prev->next);
	  while (1) {
	    v = TX_LOAD((key_type *) &next->key);
	    if (v >= key)
	      break;
	    prev = next;
	    next = (node_t *)TX_LOAD(&prev->next);
	  }		  
	  TX_END;
	  result = (v == key);

	}


#elif defined LOCKFREE			
	result = harris_find(set, key);
#endif	
	
	return result;
}

inline int set_seq_add(intset_t *set, key_type key)
{
	int result;
	node_t *prev, *next;
	
	prev = set->head;
	next = prev->next;
	while (next->key < key) {
		prev = next;
		next = prev->next;
	}
	result = (next->key != key);
	if (result) {
		prev->next = new_node(key, key, next, 0);
	}
	return result;
}	
		

int set_add(intset_t *set, key_type key, val_type val, int transactional)
{
	int result;
	
#ifdef DEBUG
	printf("++> set_add(%d)\n", (int)key);
	IO_FLUSH;
#endif

	if (!transactional) {
		
		result = set_seq_add(set, key);
		
	} else { 
	
#ifdef SEQUENTIAL /* Unprotected */
		
		result = set_seq_add(set, key);
		
#elif defined STM
	
		node_t *prev, *next;
		val_type v;	
	
		if (transactional > 2) {

		  TX_START(EL);
		  prev = set->head;
		  next = (node_t *)TX_LOAD(&prev->next);
		  while (1) {
		    v = TX_LOAD((uintptr_t *) &next->key);
		    if (v >= key)
		      break;
		    prev = next;
		    next = (node_t *)TX_LOAD(&prev->next);
		  }
		  result = (v != key);
		  if (result) {
		    TX_STORE(&prev->next, new_node(key, val, next, transactional));
		  }
		  TX_END;
		  
		} else {

		  TX_START(NL);
		  prev = set->head;
		  next = (node_t *)TX_LOAD(&prev->next);
		  while (1) {
		    v = TX_LOAD((uintptr_t *) &next->key);
		    if (v >= key)
		      break;
		    prev = next;
		    next = (node_t *)TX_LOAD(&prev->next);
		  }
		  result = (v != key);
		  if (result) {
		    TX_STORE(&prev->next, new_node(key, key, next, transactional));
		  }
		  TX_END;

		}

#elif defined LOCKFREE
		result = harris_insert(set, key, val);
#endif
		
	}
	
	return result;
}

int set_remove(intset_t *set, key_type key, int transactional)
{
	int result = 0;
	
#ifdef DEBUG
	printf("++> set_remove(%d)\n", (int)key);
	IO_FLUSH;
#endif
	
#ifdef SEQUENTIAL /* Unprotected */
	
	node_t *prev, *next;

	prev = set->head;
	next = prev->next;
	while (next->key < key) {
		prev = next;
		next = prev->next;
	}
	result = (next->key == key);
	if (result) {
		prev->next = next->next;
		free(next);
	}
			
#elif defined STM
	
	node_t *prev, *next;
	val_type v;
	node_t *n;

	if (transactional > 3) {

	  TX_START(EL);
	  prev = set->head;
	  next = (node_t *)TX_LOAD(&prev->next);
	  while (1) {
	    v = TX_LOAD((uintptr_t *) &next->key);
	    if (v >= key)
	      break;
	    prev = next;
	    next = (node_t *)TX_LOAD(&prev->next);
	  }
	  result = (v == key);
	  if (result) {
	    n = (node_t *)TX_LOAD(&next->next);
	    TX_STORE(&prev->next, n);
	    FREE(next, sizeof(node_t));
	  }
	  TX_END;
	  
	} else {
	  
	  TX_START(NL);
	  prev = set->head;
	  next = (node_t *)TX_LOAD(&prev->next);
	  while (1) {
	    v = TX_LOAD((uintptr_t *) &next->key);
	    if (v >= key)
	      break;
	    prev = next;
	    next = (node_t *)TX_LOAD(&prev->next);
	  }
	  result = (v == key);
	  if (result) {
	    n = (node_t *)TX_LOAD(&next->next);
	    TX_STORE(&prev->next, n);
	    FREE(next, sizeof(node_t));
	  }
	  TX_END;
	  
	}
	
#elif defined LOCKFREE
	result = harris_delete(set, key);
#endif
	
	return result;
}


