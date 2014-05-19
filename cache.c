#include "cachefunctions.h"
#include <stdlib.h>
#include <stdio.h>

int misscount = 0;
int accesscount = 0;
int accesstime = 0;

struct block {
  int valid;
  int tag;
};

struct cache{
  int type;
  int blocksize;
  int cachesize;
  struct block cacheArr[0];
};

/*
create cache, initialize it, return a pointer to the struct
type 0: direct-mapped cache
type 1: 2-way pseudo-associative cache
type 2: 8-way set-associative cache
*/
void *createAndInitialize(int blocksize, int cachesize, int type) {
  int blocks = cachesize/blocksize;
  // mem for cache array
  struct cache *cachey=malloc(sizeof(struct block) * blocks + sizeof(struct cache));
  cachey->type = type;
  cachey->blocksize = blocksize;
  cachey->cachesize = cachesize;


  // create default array w/ 0 for all values
  struct block generic;
  generic.valid = 0;
  generic.tag = 0;
  int i;
  for (i=0; i<blocks; i++) {
    cachey->cacheArr[i] = generic;
  }
  return cachey;
}

// function that returns log_2(value)
int log_2(int value) {
  int exp = -1;
  while (value) {
    exp ++;
    value >>= 1;
  }
  return exp;
}

int power(int base, int exp) {
  int i;
  int power = 1;
  for (i=0; i<exp; i++) {
    power *= base;
  }
  return power;
}

// access cache w/ a particular address
// if address results in hit, return 1
// if it is a miss, return 0.
int accessCache(void *cache, int address) {
  accesscount++;
  accesstime++;
  struct cache *cachey = cache;
  // calculate # of bits of address are tag,
  //  set index, and block offset
  int blocks = (cachey->cachesize)/(cachey->blocksize);
  int indexsize = log_2(blocks);
  int blockoffsetsize = log_2(cachey->blocksize);
  int tagsize = 32-blockoffsetsize-indexsize;
  
  // shift and mask to find index, valid, tag
  int index = address >> blockoffsetsize;
  int mask = (power(indexsize, 2)-1);
  index = index & mask;

  // tag and valid will be inserted to block
  int tag = address > (indexsize + blockoffsetsize);
  int valid = 1;

  // miss
  //struct block *test = &cachey->cacheArr[index];
  int validtest = cachey->cacheArr[index].valid;
  //if ((cachey->cacheArr[index]).valid == 0) {
  if (validtest == 0) {
    misscount++;
    accesstime = accesstime + 100;
    //cachey->cacheArr[index] = *entry;
    cachey->cacheArr[index].valid = valid;
    cachey->cacheArr[index].tag = tag;
    return 0;
  }
  // hit
  cachey->cacheArr[index].valid = valid;
  cachey->cacheArr[index].tag = tag;
  return 1;
}

// returns # of misses that have occurred so far
int missesSoFar(void *cache) {

	return misscount;
}

// returns # of accesses that have occurred so far
int accessesSoFar(void *cache) {

	return accesscount;
}

// returns total # of cycles that all accesses have taken so far
int totalAccessTime(void *cache) {
	//return accesscount + misscount*100;
  return accesstime;
}
