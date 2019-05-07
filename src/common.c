#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "common.h"

/******************************************************************************
* 
* Add "value" to a singly-linked list defined by Head and Tail pointers
*   Returns: true on success. false on failure.
*
******************************************************************************/
bool addToList(mpz_t value, struct node **Head, struct node **Tail) {
   struct node *this;
   this = malloc(sizeof(struct node *));
   if(this == NULL) { return(false); }
   mpz_init(this->value);
   mpz_set(this->value, value);
   if(*Head == NULL) {
      /* Add the first node to the list */
      *Head = this;
      *Tail = this;
      this->next = NULL;
   }
   else {
      /* Add the new node to the end of the list */
      this->next = NULL;
      (*Tail)->next = this;
      *Tail = this;
   }
   return(true);
}

/******************************************************************************
* 
* Print the values stored in a list defined by Head and Tail pointers
*  Returns: number of primes in the list
*
******************************************************************************/
void printList(struct node **Head, struct node **Tail, FILE **fp, mpz_t nPrimes) {
   if(Head == NULL) { 
      printf("INFO: There are no primes on the list\n");
   }
   else {
      struct node *this;
      this = *Head;
      if(fp == NULL) { printf("INFO: List of primes:\n"); }
      while(this->next != NULL) {
         mpz_out_str(*fp, BASE10, this->value);
         fprintf(*fp, "\n");
         this = this->next;
         mpz_add_ui(nPrimes, nPrimes, 1);
      }
      /* The last node still needs to be printed */
      mpz_out_str(*fp, BASE10, this->value);
      fprintf(*fp, "\n");
   }
}

/******************************************************************************
* 
* Deallocated the list defined by Head and Tail pointers
*  No return value
*
******************************************************************************/
void freeList(struct node **Head, struct node **Tail) {
   struct node *this;
   this = *Head;
   while(this->next != NULL) {
      /* Move Head to the next node on the list */
      *Head = (*Head)->next;
      /* Free "this" node */
      mpz_clear(this->value);
      this->next = NULL;
      free(this);
      /* Move on to the next node */
      this = *Head;
   }
   /* Free the last node */
   mpz_clear(this->value);
   free(this);
   (*Head) = NULL;
   (*Tail) = NULL;
}

/******************************************************************************
* 
* Move "this" pointer by N nodes along the list
*  Returns: true on success. false on failure.
*
******************************************************************************/
bool move(struct node **prev, mpz_t N) {
   mpz_t idx;
   mpz_init(idx);
   mpz_set_str(idx, "0", BASE10);

   while(mpz_cmp(idx, N) < 0) {
      if((*prev)->next == NULL) {
         mpz_clear(idx);
         return(false);
      }
      else {
         (*prev) = (*prev)->next;
         mpz_add_ui(idx, idx, 1);
      }
   }
   mpz_clear(idx);
   return(true);
}
