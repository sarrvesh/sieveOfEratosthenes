#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define NUM_INPUTS 2
#define VERSION_STR "0.1"
#define BASE10 10
#define LOWEST_PRIME 2
#define DEBUG 1

struct node {
   mpz_t value;
   struct node *next;
};

bool addToList(mpz_t value, struct node **Head, struct node **Tail) {
   struct node *this;
   this = malloc(sizeof(struct node *));
   if(this == NULL) { return(false); }
   mpz_init(this->value);
   mpz_set(this->value, value);

   if(*Head == NULL) {
      /* Add the first node to the list */
      if(DEBUG) { printf("DEBUG: Adding first node\n"); }
      *Head = this;
      *Tail = this;
      this->next = NULL;
   }
   else {
      /* Add the new node to the end of the list */
      if(DEBUG) { printf("DEBUG: Adding node to existing list\n"); }
      this->next = NULL;
      (*Tail)->next = this;
      *Tail = this;
   }

   if(DEBUG) {
      printf("DEBUG: Add to list ");
      mpz_out_str(NULL, BASE10, this->value);
      printf("   ");
      mpz_out_str(NULL, BASE10, value);
      printf("\n");
   }

   return(true);
}

void printThisNode(struct node *this) {
   printf("   ");
   mpz_out_str(NULL, BASE10, this->value);
   printf("\n");
}

void printList(struct node **Head, struct node **Tail) {
   if(Head == NULL) { 
      printf("INFO: There are no primes on the list\n");
   }
   else {
      struct node *this;
      this = *Head;
      printf("INFO: List of primes:\n");
      while(this->next != NULL) {
         printThisNode(this);
         this = this->next;
      }
      /* The last node still needs to be printed */
      printThisNode(this);
   }
}

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
* Function to move "this" pointer by N nodes along the list
*
******************************************************************************/
void move(struct node **prev, mpz_t N) {
   mpz_t idx;
   mpz_init(idx);
   mpz_set_str(idx, "0", BASE10);
   while(mpz_cmp(idx, N) < 0) {
      (*prev) = (*prev)->next;
      mpz_add_ui(idx, idx, 1);
   }
   mpz_clear(idx);
}

/******************************************************************************
*
* Function to check if the I can make a new hop. This is done by testing if the 
*   distance between "this" node and "Tail" node is less than "hopSize".
*
******************************************************************************/
bool isNextHopValid(struct node *this, struct node *Tail, mpz_t hopSize) {
   mpz_t thisValue, tailValue, diff;
   mpz_init(thisValue);
   mpz_init(tailValue);
   mpz_init(diff);
   bool retValue = false;

   mpz_set(thisValue, this->value);
   mpz_set(tailValue, Tail->value);
   mpz_sub(diff, tailValue, thisValue);

   if(mpz_cmp(diff, hopSize) > 0) { retValue = true; }

   /* Free the GMP variables before returning */
   mpz_clear(thisValue);
   mpz_clear(tailValue);
   mpz_clear(diff);

   return(retValue);
}

/******************************************************************************
*
* Apply the sieve to a node represented by the Head and the Tail pointers.
*  By this function ends, the list should only contain prime numbers.
*
******************************************************************************/
void doSieve(struct node **Head, struct node **Tail) {
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
      printf("DEBUG: thisIter value is ");
      mpz_out_str(NULL, BASE10, hopSize);
      mpz_sub_ui(hopSize_1, hopSize, 1);
      if(DEBUG) {
         printf("DEBUG: thisIter/prev/next   ");
         mpz_out_str(NULL, BASE10, thisIter->value);
         printf("  ");
         mpz_out_str(NULL, BASE10, prev->value);
         printf("  ");
         mpz_out_str(NULL, BASE10, next->value);
         printf("\n");
      }
      while(isNextHopValid(prev, *Tail, hopSize)) {
         /* Move next by hopSize and prev by hopSize-1 */
         move(&prev, hopSize_1);
         next = prev->next;
         if(DEBUG) {
            printf("DEBUG: thisIter/prev/next   ");
            mpz_out_str(NULL, BASE10, thisIter->value);
            printf("  ");
            mpz_out_str(NULL, BASE10, prev->value);
            printf("  ");
            mpz_out_str(NULL, BASE10, next->value);
         }
         /* Remove the node pointed by next from the list */
         prev->next = next->next;
         next->next = NULL;
         mpz_clear(next->value);
         free(next);
         next = prev->next;
         if(DEBUG) {
            printf("\nDEBUG: thisIter/prev/next   ");
            mpz_out_str(NULL, BASE10, thisIter->value);
            printf("  ");
            mpz_out_str(NULL, BASE10, prev->value);
            printf("  ");
            mpz_out_str(NULL, BASE10, next->value);         
            printf("\n\n\n");
         }
      }
      break;
      thisIter = thisIter->next;
   }
}

/******************************************************************************
* 
* Main function
*
******************************************************************************/
int main(int argc, char *argv[]) {

   /* Variable declarations */
   struct node *Head = NULL;
   struct node *Tail = NULL;
   clock_t start, end;
   float createT, printT, freeT, sieveT;

   /* Declaration GMP variables and initialize them */
   mpz_t upperBound, loopIdx;
   mpz_init(upperBound);
   mpz_init(loopIdx);

   printf("\n");
   printf("Sieve of Eratosthenes v%s\n", VERSION_STR);
   printf("Written by Sarrvesh S. Sridhar\n\n");

   if(DEBUG) {
      printf("DEBUG: Compiled with GNU MP version %d.%d\n",
             __GNU_MP_VERSION, __GNU_MP_VERSION_MINOR);
      printf("DEBUG: GNU MP built with %s using flags:\n", __GMP_CC);
      printf("   ");
      puts(__GMP_CFLAGS);
      printf("\n");
   }

   /* Parse the command line input */
   if(argc != NUM_INPUTS) {
      printf("ERROR: Invalid command line input. Terminating execution!\n");
      printf("Usage: %s <limit>\n\n", argv[0]);
      exit(1);
   }
   /* Create a GMP type for input upperBound */
   mpz_set_str(upperBound, argv[1], BASE10);

   if(DEBUG) {
      printf("DEBUG: Input value is ");
      mpz_out_str(NULL, BASE10, upperBound);
      printf("\n");
   }

   /* Initialize the list */
   start = clock();
   mpz_set_str(loopIdx, "2", BASE10);
   while(mpz_cmp(loopIdx, upperBound) < 0) {
      addToList(loopIdx, &Head, &Tail);
      mpz_add_ui(loopIdx, loopIdx, 1);
   }
   end = clock();
   createT = (float)(end - start)/CLOCKS_PER_SEC;

   /* Parse the list and remove nodes containing non-prime numbers */
   start = clock();
   doSieve(&Head, &Tail);
   end = clock();
   sieveT = (float)(end - start)/CLOCKS_PER_SEC;

   /* Print the final list of prime numbers */
   start = clock();
   printList(&Head, &Tail);
   end = clock();
   printT = (float)(end - start)/CLOCKS_PER_SEC;

   /* Free the memory allocated to the list */
   start = clock();
   freeList(&Head, &Tail);
   end = clock();
   freeT = (float)(end - start)/CLOCKS_PER_SEC;

   /* Clear the GMP variables */
   mpz_clear(upperBound);
   mpz_clear(loopIdx);

   /* Report execution time */
   printf("INFO: Time to initialize the list: %f seconds\n", createT);
   printf("INFO: Time to sieve: %f seconds\n", sieveT);
   printf("INFO: Time to print: %f seconds\n", printT);
   printf("INFO: Time to free the list: %f seconds\n", freeT);

}
