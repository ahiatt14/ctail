#include <math.h>
#include <string.h>
#include <float.h>

#include <stdio.h> //TODO: remove

#include "public_types.h"
#include "precision.h"
#include "tail_math.h"
#include "vec3.h"

float deg_to_rad(float d) {
  return d * M_PI / 180;
}

int iclamp(int v, int min, int max) {
  return fmin(max, fmax(v, min));
}

void calculate_face_normal(
  const vec3 *ccw_triangle_positions,
  vec3 *cross_product
) {
  vec3 temp_edge1 = {0};
  vec3 temp_edge2 = {0};
  vec3_minus_vec3(
    &ccw_triangle_positions[2],
    &ccw_triangle_positions[0],
    &temp_edge1
  );
  vec3_minus_vec3(
    &ccw_triangle_positions[0],
    &ccw_triangle_positions[1],
    &temp_edge2
  );
  vec3_cross(
    &temp_edge1,
    &temp_edge2,
    cross_product
  );
  vec3_normalize(cross_product, cross_product);
}

void copy_vec3s(
  const vec3 *vec3s,
  const unsigned int *indices,
  int index_count,
  vec3* copied_vec3s
) {
  for (int i = 0; i < index_count; i++) memcpy(
    &copied_vec3s[i],
    &vec3s[indices[i]].x,
    sizeof(vec3)
  );
}

static float_tolerance ft = {
  .tolerance = FLT_EPSILON,
  .within_tolerance = diff_is_within_tolerance
};
static inline int contains_vec3(
  const vec3 *t,
  int count,
  const vec3 *vec3s
) {
  for (int i = 0; i < count; i++) {
    if (vec3_equals_vec3(t, &vec3s[i], &ft)) return 1;
  }
  return 0;
}

// static void print_v3(vec3 *t) {
//   printf(" { %.6f, %.6f, %.6f } \n", t->x, t->y, t->z);
// }

// TODO: not optimized (or safe?) to use in real time, revisit if needed
void calculate_vertex_normal(
  unsigned short vertex_index,
  unsigned short index_buffer_count,
  const unsigned int *indices,
  const vec3 *positions,
  vec3 *normalized_output
) {
  vec3 temp_cross = {0};
  vec3 cross_vectors[100] = {0};
  int cross_count = 0;
  int start_of_current_triangle = 0;
  vec3 temp_triangle_positions[3] = {0};
  for (int i = 0; i < index_buffer_count; i++) {
    if (indices[i] != vertex_index) continue;
    start_of_current_triangle = floor(i / 3) * 3;
    copy_vec3s(
      positions,
      &indices[start_of_current_triangle],
      3,
      temp_triangle_positions
    );
    calculate_face_normal(
      temp_triangle_positions,
      &temp_cross
    );
    if (!contains_vec3(
      &temp_cross,
      cross_count,
      cross_vectors
    )) memcpy(
      &cross_vectors[cross_count++],
      &temp_cross,
      sizeof(vec3)
    );
  }
  vec3_mean(cross_vectors, cross_count, normalized_output);
  vec3_normalize(normalized_output, normalized_output);
}