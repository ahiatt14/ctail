#ifndef __TAIL_MATH__
#define __TAIL_MATH__

#include "public_types.h"

extern const float PI;

float deg_to_rad(float d);

int iclamp(int v, int min, int max);

void calculate_normal(
  unsigned short vertex_index,
  unsigned short index_buffer_length,
  const unsigned int *indices,
  const vec3 *positions,
  vec3 *normalized_output
);

#endif