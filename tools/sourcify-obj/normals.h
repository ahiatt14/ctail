#ifndef __TAIL_PARSER_NORMALS__
#define __TAIL_PARSER_NORMALS__

#include "vector.h"

Vec3 calculate_face_normal(
  Vec3 const *const ccw_triangle_positions
);

Vec3 calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  const unsigned int *indices,
  const Vec3 *positions
);

#endif