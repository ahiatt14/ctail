#include <math.h>
#include <float.h>

#include "tail_math.h"
#include "precision.h"

float deg_to_rad(float d) {
  return d * M_PI / 180;
}

int vec_equals_vec(
  float const *const x0,
  float const *const x1,
  unsigned short int element_count,
  struct float_tolerance const *const ft
) {
  for (int i = 0; i < element_count; i++) {
    if (!ft->within_tolerance(
      x0[i],
      x1[i],
      ft->tolerance
    )) return 0;
  }
  return 1;
}

int iclamp(int v, int min, int max) {
  return fmin(max, fmax(v, min));
}