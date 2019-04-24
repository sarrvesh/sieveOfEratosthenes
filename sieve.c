#include <stdio.h>
#include <gmp.h>

#define NUM_INPUTS 2
#define VERSION_STR "0.1"
#define BASE10 10
#define DEBUG 1

int main(int argc, char *argv[]) {

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
   }
   /* Create a GMP type for input upperBound */
   mpz_set_str(upperBound, argv[1], BASE10);

   if(DEBUG) {
      printf("DEBUG: Input value is\n");
      mpz_out_str(NULL, BASE10, upperBound);
      printf("\n");
   }

   /* Start the loop */
   mpz_set_str(loopIdx, "2", BASE10);
   while(mpz_cmp(loopIdx, upperBound) < 0) {
      printf("This num is ");
      mpz_out_str(NULL, BASE10, loopIdx);
      printf("\n");
      mpz_add_ui(loopIdx, loopIdx, 1);
   }

   /* Clear the GMP variables */
   mpz_clear(upperBound);
   mpz_clear(loopIdx);
}
