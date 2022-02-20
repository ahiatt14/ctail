#include <math.h>
#include <float.h>

#include "tail_math.h"

float deg_to_rad(float d) {
  return d * M_PI / 180;
}

int iclamp(int v, int min, int max) {
  return fmin(max, fmax(v, min));
}