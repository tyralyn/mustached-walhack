#include <stdio.h>
#include <stdlib.h>
#include "cachefunctions.h"

int main() {

  int blocksize = 16;
  int cachesize = 64;
  int address = 0x22657C;
  int type = 0;

  struct cache *c = createAndInitialize(blocksize, cachesize, type);
  accessCache(c, address);

  //int misses = missesSoFar(c);
  //int accesses = accessesSoFar(c);
  //int accesstime = totalAccessTime(c);


  //printf("number of misses = %d\n", misses);
  //printf("number of accesses = %d\n", accesses);
  //printf("access time = %d\n", accesstime);

  return 0;
}
