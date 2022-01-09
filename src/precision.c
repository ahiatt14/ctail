#include <math.h>
#include <float.h>
#include "precision.h"

int diff_is_within_mag_based_tolerance(float a, float b, float tolerance) {
  float diff = fabs(a - b);
  float abs_a = fabs(a);
  float abs_b = fabs(b);
  float max = fmax(abs_a, abs_b);
  return (diff <= max * tolerance) ? 1 : 0;
}