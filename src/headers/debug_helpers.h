#ifndef __TAIL_DEBUG_HELPERS__
#define __TAIL_DEBUG_HELPERS__

#include <stdio.h>

void print_m4x4(const char *name, const m4x4 *m) {
  const float *data = &(m->data[0]);
  printf("%s\n", name);
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      printf("%.12f, ", data[r * 4 + c]);
    }
    printf("\n");
  }
}

void print_vec4(const char *name, const vec4 *t) {
  printf("%s\n", name);
  for (int i = 0; i < 4; i++)
    printf("%.12f, ", (&t->x)[i]);
  printf("\n");
}

#endif