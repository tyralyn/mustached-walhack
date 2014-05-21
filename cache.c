#include "cachefunctions.h"
#include <stdlib.h>
#include <stdio.h>

int misscount = 0;
int accesscount = 0;
int accesstime = 0;

struct block {
  int valid; //whether content in block is actual content
  int tag; //distinguishes item in index 
  int index; //the index for the block or set -- has associativity number of blocks in it
  int indexindex; //index of block in index
};

struct cache{
  int type; //direct map (0), 2-way pseudoassociative (1), 8-way associative (2)
  int blocksize; //size of block in bits/bytes
  int cachesize; //size of cache in bits or bytes
  int associativity; //number of blocks per set -- 1 if direct map
  int numblocks; //number of blocks in cache
  // int setcount; //number if blocks in set
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
  cachey->numblocks = blocks;//cachesize/blocksize;
  if (cachey->type == 0) { cachey->associativity = 1; }
  else if (cachey->type == 1) { cachey->associativity = 2; }
  else if (cachey->type == 2) { cachey->associativity = 8; }
  else;
  
  

  struct block generic;
  generic.valid = 0;
  generic.tag = 0;
  int i;
  int index=0, indexindex=0;
  for (i=0; i<blocks; i++) {
    cachey->cacheArr[i] = generic;
    cachey->cacheArr[i].index = index;
    indexindex = i % cachey->associativity;
    cachey->cacheArr[i].indexindex = indexindex;
    index = (int)i/cachey->associativity;
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
  /* accesstime++; */
  struct cache *cachey = cache;
  // calculate # of bits of address are tag,
  //  set index, and block offset
  int blocks = cachey->numblocks;
  int associativity = cachey->associativity;
  //if (cachey->type == 0) {

  int indexsize = log_2((int)blocks/associativity);
  int blockoffsetsize = log_2(cachey->blocksize);
  int tagsize = 32-blockoffsetsize-indexsize;
  
  // shift and mask to find index, valid, tag
  int index = address >> blockoffsetsize;
  int mask = (power(indexsize, 2)-1);
  index = index & mask;

  // tag and valid will be inserted to block
  int tag = address > (indexsize + blockoffsetsize);
  int valid = 1;
  //printf("index if direct mapped %d\n", index);
  // miss
  //struct block *test = &cachey->cacheArr[index];
  //int validtest = cachey->cacheArr[index].valid;
  //if ((cachey->cacheArr[index]).valid == 0) { 

  if (cachey->type == 0) { 
    accesstime++;  
    int validtest = cachey->cacheArr[index].valid;
    if (validtest == 0) {
      misscount++;
      accesstime = accesstime + 100;
      //cachey->cacheArr[index] = *entry;
      cachey->cacheArr[index].valid = valid;
      cachey->cacheArr[index].tag = tag;
      return 0;
    }
    else {
      if (tag == cachey->cacheArr[index].tag) {
	return 1;
      }
      else {
	misscount++;
	accesstime = accesstime+100;
	cachey->cacheArr[index].valid = valid;
	cachey->cacheArr[index].tag = tag;
	return 0;
      }
    }
  }
  //printf("index if direct mapped %d\n", index);
  else if (cachey->type ==1) {
    accesstime++;
    int firstBlock = index*2;
    //printf("index if 2way %d\n", f);
    int validtest = cachey->cacheArr[firstBlock].valid;
    int validtest2= cachey->cacheArr[firstBlock+1].valid;
    if (validtest==0) {
      if (validtest2==0) {
	misscount++;
	accesstime = accesstime+100;
	cachey->cacheArr[firstBlock+1].valid = cachey->cacheArr[firstBlock].valid;
	cachey->cacheArr[firstBlock+1].tag=cachey->cacheArr[firstBlock].tag;
	cachey->cacheArr[firstBlock].valid = valid;
	cachey->cacheArr[firstBlock].tag=tag;
	return 0;
      }
      int validHold = cachey->cacheArr[firstBlock+1].valid;
      int tagHold = cachey->cacheArr[firstBlock+1].tag;
      cachey->cacheArr[firstBlock+1].valid = cachey->cacheArr[firstBlock].valid;
      cachey->cacheArr[firstBlock+1].tag=cachey->cacheArr[firstBlock].tag;
      cachey->cacheArr[firstBlock].valid = validHold;
      cachey->cacheArr[firstBlock].tag=tagHold;
      //accesstime++; 
      return 1;
    }
    //accesstime++; 
    cachey->cacheArr[firstBlock].valid = valid;
    cachey->cacheArr[firstBlock].tag = tag;
    return 1;
  }
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
