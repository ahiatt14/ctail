#ifndef __TAIL_OBJ_PARSER__
#define __TAIL_OBJ_PARSER__

#include <stdlib.h>

#include "tail.h"

void obj_float_line_to_vector(const char *obj_line, float *x);
void obj_f_line_to_3_ui_indices(
  const char *obj_line,
  unsigned int *indices
);
void obj_f_triplet_line_to_vec3s(
  const char *obj_line,
  float *v_indices,
  float *vn_indices,
  float *vt_indices
);
uint8_t obj_shading_is_flat(
  FILE *obj_file
);
void parse_obj_into_smooth_mesh(
  FILE *obj_file,
  struct vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
);
void parse_obj_into_flat_mesh(
  FILE *obj_file,
  struct vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
);

#endif