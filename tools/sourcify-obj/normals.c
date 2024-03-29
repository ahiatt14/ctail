#include <string.h>
#include <float.h>
#include <math.h>

#include "normals.h"

#include "tail_math.h"
#include "precision.h"
#include "vector.h"

Vec3 calculate_face_normal(
  Vec3 const *const ccw_triangle_positions
) {
  Vec3 temp_edge1 = vec3_minus_vec3(
    ccw_triangle_positions[2],
    ccw_triangle_positions[0]
  );
  Vec3 temp_edge2 = vec3_minus_vec3(
    ccw_triangle_positions[0],
    ccw_triangle_positions[1]
  );
  return vec3__normalize(vec3__cross(
    temp_edge1,
    temp_edge2
  ));
}

static void copy_vec3s(
  Vec3 const *const vec3s,
  const unsigned int *indices,
  int index_count,
  Vec3 *const copied_vec3s
) {
  for (int i = 0; i < index_count; i++) memcpy(
    &copied_vec3s[i],
    &vec3s[indices[i]].x,
    sizeof(Vec3)
  );
}

static FloatTolerance ft = {
  .tolerance = FLT_EPSILON,
  .within_tolerance = diff_is_within_tolerance
};
static inline int contains_vec3(
  Vec3 t,
  int count,
  Vec3 const *const vec3s
) {
  for (int i = 0; i < count; i++) {
    if (vec_equals_vec(&t.x, &vec3s[i].x, 3, &ft)) return 1;
  }
  return 0;
}

// TODO: not optimized (or safe?) to use in real time, revisit if needed
Vec3 calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  unsigned int const *const indices,
  Vec3 const *const positions
) {
  Vec3 temp_cross = {0};
  Vec3 cross_vectors[100] = {0};
  int cross_count = 0;
  int start_of_current_triangle = 0;
  Vec3 temp_triangle_positions[3] = {0};
  for (int i = 0; i < index_buffer_count; i++) {
    if (indices[i] != vertex_index) continue;
    start_of_current_triangle = floor(i / 3) * 3;
    copy_vec3s(
      positions,
      &indices[start_of_current_triangle],
      3,
      temp_triangle_positions
    );
    temp_cross = calculate_face_normal(temp_triangle_positions);
    if (!contains_vec3(
      temp_cross,
      cross_count,
      cross_vectors
    )) memcpy(
      &cross_vectors[cross_count++],
      &temp_cross,
      sizeof(Vec3)
    );
  }
  return vec3__normalize(vec3__mean(cross_vectors, cross_count));
}