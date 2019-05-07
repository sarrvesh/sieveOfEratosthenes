#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <gmp.h>
#include <stdio.h>

struct node {
   mpz_t value;
   struct node *next;
};

bool move(struct node **prev, mpz_t N);
bool addToList(mpz_t value, struct node **Head, struct node **Tail);
void printList(struct node **Head, struct node **Tail, FILE **fp, mpz_t nPrimes);
void freeList(struct node **Head, struct node **Tail);
bool move(struct node **prev, mpz_t N);

#endif
