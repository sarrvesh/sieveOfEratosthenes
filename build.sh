GMP=/home/darkknight/Documents/gmp_6.1.2/


gcc -Wall -c -L${GMP}/lib -I${GMP}/include sieve.c
gcc -O2 -march=native -L${GMP}/lib -I${GMP}/include sieve.o -o sieve -lgmp
