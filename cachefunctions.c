#include <cachefunctions.h>

/*
create cache, initialize it, return a pointer to the struct
type 0: direct-mapped cache
type 1: 2-way pseudo-associative cache
type 2: 8-way set-associative cache
*/
void *createAndInitialize(int blocksize, int cachesize, int type) {

}

// access cache w/ a particular address
// if address results in hit, return 1
// if it is a miss, return 0.
int accessCache(void *cache, int address) {

	return 0;
}

// returns # of misses that have occurred so far
int missesSoFar(void *cache) {

	return 0;
}

// returns # of accesses that have occurred so far
int accessesSoFar(void *cache) {

	return 0;
}

// returns total # of cycles that all accesses have taken so far
int totalAccessTime(void *cache) {

	return 0;
}
