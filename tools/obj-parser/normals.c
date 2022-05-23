#include <string.h>
#include <float.h>
#include <math.h>

#include "tail.h"
#include "normals.h"

void calculate_face_normal(
  const struct vec3 *ccw_triangle_positions,
  struct vec3 *cross_product
) {
  struct vec3 temp_edge1 = {0};
  struct vec3 temp_edge2 = {0};
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

static void copy_vec3s(
  const struct vec3 *vec3s,
  const unsigned int *indices,
  int index_count,
  struct vec3 *copied_vec3s
) {
  for (int i = 0; i < index_count; i++) memcpy(
    &copied_vec3s[i],
    &vec3s[indices[i]].x,
    sizeof(struct vec3)
  );
}

static struct float_tolerance ft = {
  .tolerance = FLT_EPSILON,
  .within_tolerance = diff_is_within_tolerance
};
static inline int contains_vec3(
  const struct vec3 *t,
  int count,
  const struct vec3 *vec3s
) {
  for (int i = 0; i < count; i++) {
    if (vec_equals_vec(&t->x, &vec3s[i].x, 3, &ft)) return 1;
  }
  return 0;
}

// TODO: not optimized (or safe?) to use in real time, revisit if needed
void calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  const unsigned int *indices,
  const struct vec3 *positions,
  struct vec3 *normalized_output
) {
  struct vec3 temp_cross = {0};
  struct vec3 cross_vectors[100] = {0};
  int cross_count = 0;
  int start_of_current_triangle = 0;
  struct vec3 temp_triangle_positions[3] = {0};
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
      sizeof(struct vec3)
    );
  }
  vec3_mean(cross_vectors, cross_count, normalized_output);
  vec3_normalize(normalized_output, normalized_output);
}