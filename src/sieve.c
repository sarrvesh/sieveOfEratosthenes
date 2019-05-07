#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <argp.h>

#define BASE10 10

struct node {
   mpz_t value;
   struct node *next;
};

/* Global variables for argument parser */
const char *argp_program_version = "sieve 0.1";
const char *argp_program_bug_address = "<sarrvesh.ss@gmail.com>";
static char doc[] = 
    "Generate prime numbers using various sieving methods.";

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

struct arguments {
    char *limit;
    char *outFileName;
    char *method;
    FILE *fp;
};

/******************************************************************************
* 
* Function to handle command line input
*
******************************************************************************/
static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;   
    switch(key) {
        case 'm':
            arguments->method = arg;
            break;
        case 'o':
            arguments->outFileName = arg;
            break;
        case 'l':
            arguments->limit = arg;
            break;
    }
   return(0);
}

/******************************************************************************
* 
* Validate the parsed command line input
*
******************************************************************************/
void validateCLI(struct arguments *arguments) {
   if(arguments->limit == NULL) {
      printf("ERROR: Option -l undefined\n");
      printf("Try `sieve --help' or `sieve --usage' for more information.\n\n");
      exit(1);
   }
   if(arguments->outFileName == NULL) {
      printf("INFO: Option -o undefined\n");
      printf("INFO: Output will be redirected to standard output.\n");
      arguments->fp = stdout;
   }
   else {
      arguments->fp = fopen(arguments->outFileName, "w+");
      if(arguments->fp == NULL) {
         printf("ERROR: Unable to open output file.\n");
         printf("ERROR: Terminating execution\n");
         exit(0);
      }
   }
   if(arguments->method == NULL) {
      printf("INFO: Option -e undefined. Defaulting to method 'e'.\n");
      arguments->method = "e";
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

   /* Parse the command line input */
   struct arguments arguments = { 0 };
   static struct argp_option options[] = {
    {0, 'l', "limit",  0, "Largest number upto which the code will\nlist prime numbers."},
    {0, 'o', "output", 0, "Name of the output file."},
    {0, 'm', "method", 0, "Type of sieving method to generate prime numbers.\nValid options are:\n   e (for Sieve of Eratosthenes)."},
    {0} // An empty terminating record
   };
   struct argp argp = {options, parse_opt, 0, doc};
   argp_parse(&argp, argc, argv, 0, 0, &arguments);
   validateCLI(&arguments);

   //printf("DEBUG: Compiled with GNU MP version %d.%d\n",
   //          __GNU_MP_VERSION, __GNU_MP_VERSION_MINOR);
   //printf("DEBUG: GNU MP built with %s using flags:\n", __GMP_CC);
   //printf("   ");
   //puts(__GMP_CFLAGS);
   //printf("\n");
      
   /* Convert the input limit to a GMP variable and initialize */
   mpz_t upperBound, loopIdx, nPrimes;
   mpz_init(upperBound);
   mpz_init(loopIdx);
   mpz_init(nPrimes);
   mpz_set_str(upperBound, arguments.limit, BASE10);

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
   printf("INFO: Generating primes... Please wait\n");
   doSieveEratoshthenes(&Head, &Tail);
   end = clock();
   sieveT = (float)(end - start)/CLOCKS_PER_SEC;

   /* Print the final list of prime numbers */
   start = clock();
   printList(&Head, &Tail, &(arguments.fp), nPrimes);
   printf("INFO: Found ");
   mpz_out_str(NULL, BASE10, nPrimes);
   printf(" primes less than %s\n", arguments.limit);
   if(arguments.fp != stdout) { fclose(arguments.fp); }
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
   mpz_clear(nPrimes);

   /* Report execution time */
   printf("INFO: Time to initialize the list: %f seconds\n", createT);
   printf("INFO: Time to sieve: %f seconds\n", sieveT);
   printf("INFO: Time to print: %f seconds\n", printT);
   printf("INFO: Time to free the list: %f seconds\n", freeT);

}
