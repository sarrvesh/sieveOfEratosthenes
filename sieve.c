#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define NUM_INPUTS 2
#define VERSION_STR "0.1"
#define BASE10 10
#define LOWEST_PRIME 2
#define DEBUG 0

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

int main(int argc, char *argv[]) {

   /* Variable declarations */
   struct node *Head = NULL;
   struct node *Tail = NULL;
   clock_t start, end;
   float createT, printT, freeT;

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
   printf("INFO: Time to print: %f seconds\n", printT);
   printf("INFO: Time to free the list: %f seconds\n", freeT);

}
