#include <gmp.h>
#include <stdlib.h>
#include "common.h"

/******************************************************************************
*
* Apply the sieve to a node represented by the Head and the Tail pointers.
*  By this function ends, the list should only contain prime numbers.
*
******************************************************************************/
void doSieveEratoshthenes(struct node **Head, struct node **Tail) {
   mpz_t hopSize, hopSize_1;
   mpz_init(hopSize);
   mpz_init(hopSize_1);
   struct node *thisIter, *prev, *next;
   thisIter = *Head;
   /* Loop over each node and remove the node.value'th nodes */
   while(thisIter != *Tail) {
      prev = thisIter;
      next = thisIter;
      /* How many nodes should I hop? */
      mpz_set(hopSize, thisIter->value);
      mpz_sub_ui(hopSize_1, hopSize, 1);
      while(true) {
         /* Move next by hopSize and prev by hopSize-1 */
         if(move(&prev, hopSize_1)) {
            /* If prev is on the last node, quit this loop */
            if(prev == *Tail) { break; }
            else { next = prev->next; }
            /* If next is on the last node, remove this node and quit the loop */
            if(next == *Tail) { 
               prev->next = NULL;
               *Tail = prev;
               mpz_clear(next->value);
               free(next);
               break; 
            }
            /* Remove the node pointed by next from the list */
            prev->next = next->next;
            next->next = NULL;
            mpz_clear(next->value);
            free(next);
            next = prev->next;
         }
         else { break; }
      }
      if(thisIter->next != NULL) { thisIter = thisIter->next; }
   }
}
