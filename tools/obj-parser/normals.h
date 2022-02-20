#ifndef __TAIL_PARSER_NORMALS__
#define __TAIL_PARSER_NORMALS__

void calculate_face_normal(
  const vec3 *ccw_triangle_positions,
  vec3 *cross_product
);

void calculate_vertex_normal(
  int vertex_index,
  int index_buffer_count,
  const unsigned int *indices,
  const vec3 *positions,
  vec3 *normalized_output
);



#endif