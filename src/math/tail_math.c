#include <math.h>
#include "tail_math.h"

float deg_to_rad(float d) {
  return d * M_PI / 180;
}

int iclamp(int v, int min, int max) {
  return fmin(max, fmax(v, min));
}

void calculate_normal(
  unsigned short vertex_index,
  unsigned short index_buffer_length,
  const unsigned int *indices,
  const vertex *vertices,
  vec3 *normalized_output
) {
  
}