#ifndef __TAIL_PARSER_NORMALS__
#define __TAIL_PARSER_NORMALS__

#include "vector.h"

struct vec3 calculate_face_normal(
  struct vec3 const *const ccw_triangle_positions
);

struct vec3 calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  const unsigned int *indices,
  const struct vec3 *positions
);



#endif