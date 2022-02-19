#ifndef __TAIL_MATH__
#define __TAIL_MATH__

#include "vec3.h"

float deg_to_rad(float d);

int iclamp(int v, int min, int max);

void copy_vec3s(
  const vec3 *vec3s,
  const unsigned int *indices,
  int index_count,
  vec3* copied_vec3s
);

void calculate_face_normal(
  const vec3 *ccw_triangle_positions,
  vec3 *normalized_normal
);

void calculate_vertex_normal(
  int vertex_index,
  int index_buffer_length,
  const unsigned int *indices,
  const vec3 *positions,
  vec3 *normalized_output
);

#endif