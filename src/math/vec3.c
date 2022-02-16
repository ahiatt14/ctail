#include <stdio.h>
#include "public_types.h"
#include "vec3.h"

void vec3_minus_vec3(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *diff
) {
  diff->x = t0->x - t1->x;
  diff->y = t0->y - t1->y;
  diff->z = t0->z - t1->z;
}

void vec3_plus_vec3(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *sum
) {
  sum->x = t0->x + t1->x;
  sum->y = t0->y + t1->y;
  sum->z = t0->z + t1->z;
}

void vec3_create(float x, float y, float z, vec3 *t) {
  t->x = x; t->y = y; t->z = z;
}

void vec3_mean(const vec3 *vec3s, int count, vec3 *avg) {
  for (int component = 0; component < 3; component++) {
    float temp_mean = 0;
    for (int i = 0; i < count; i++) {
      const vec3 *current_vec3 = &vec3s[i];
      float v = (&current_vec3->x)[component];
      temp_mean += v;
    }
    (&avg->x)[component] = temp_mean / count;
  }
}

      // printf(
      //   "\ncomp: %i -- v: %.8f -- temp mean: %.8f",
      //   component,
      //   v,
      //   temp_mean
      // );
      // printf("\n");