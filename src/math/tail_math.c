#include <math.h>
#include <string.h>

#include "public_types.h"
#include "tail_math.h"
#include "vec3.h"

float deg_to_rad(float d) {
  return d * M_PI / 180;
}

int iclamp(int v, int min, int max) {
  return fmin(max, fmax(v, min));
}

static inline void vec3_midpoint(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *mid
) {
  vec3 pair[2] = { *t0, *t1 };
  vec3_mean(pair, 2, mid);
}

// TODO: surely not optimized for realtime use, revisit if needed
void calculate_normal(
  unsigned short vertex_index,
  unsigned short index_buffer_count,
  const unsigned int *indices,
  const vec3 *positions,
  vec3 *normalized_output
) {
  vec3 far_edge_midpoint = {0};
  vec3 midpoint_to_target_position = {0};
  vec3 normal = {0};
  int targets_index_in_current_triangle = 0;
  int adjacent_triangles_count;

  for (int i = 0; i < index_buffer_count; i++) {
    if (indices[i] != vertex_index) continue;

    targets_index_in_current_triangle = i % 3;

    switch (targets_index_in_current_triangle) {
      case 0:
        vec3_midpoint(
          &positions[i + 1],
          &positions[i + 2],
          &far_edge_midpoint
        );
      break;
      case 1:
        vec3_midpoint(
          &positions[i - 1],
          &positions[i + 1],
          &far_edge_midpoint
        );
      break;
      case 2:
        vec3_midpoint(
          &positions[i - 1],
          &positions[i - 2],
          &far_edge_midpoint
        );
      break;
    }
    vec3_minus_vec3(
      &far_edge_midpoint,
      &positions[i],
      &midpoint_to_target_position
    );
    vec3_plus_vec3(
      &normal,
      &midpoint_to_target_position,
      &normal
    );
    adjacent_triangles_count++;
  }

  // average
  normal.x = normal.x / adjacent_triangles_count;
  normal.y = normal.y / adjacent_triangles_count;
  normal.z = normal.z / adjacent_triangles_count;

  vec3_normalize(&normal, &normal);

  memcpy(&normalized_output->x, &normal.x, sizeof(vec3));
}