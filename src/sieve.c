#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <argp.h>

#include "common.h"
#include "eratosthenes.h"
#include "constants.h"

struct arguments {
    char *limit;
    char *outFileName;
    char *method;
    FILE *fp;
};

/* Global variables for argument parser */
const char *argp_program_version = "sieve 0.1";
const char *argp_program_bug_address = "<sarrvesh.ss@gmail.com>";
static char doc[] = 
    "Generate prime numbers using various sieving methods.";

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
