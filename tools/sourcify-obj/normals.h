#ifndef __TAIL_PARSER_NORMALS__
#define __TAIL_PARSER_NORMALS__

#include "vector.h"

struct Vec3 calculate_face_normal(
  struct Vec3 const *const ccw_triangle_positions
);

struct Vec3 calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  const unsigned int *indices,
  const struct Vec3 *positions
);



#endif