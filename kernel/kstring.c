#include "kstring.h"

void* memset(void* dst, i32 c, u32 n) {
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}
