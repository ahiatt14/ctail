#ifndef __TAIL_OBJ_PARSER__
#define __TAIL_OBJ_PARSER__

#define OBJ_LINE_MAX_LENGTH 100

#include "tail.h"

void print_vert(const vertex *v);
void print_vec3(const vec3 *t);
void obj_vec3_line_to_vec3(const char *obj_line, vec3 *t);
void obj_f_line_to_3_ui_indices(
  const char *obj_line,
  unsigned int *indices
);
void obj_f_n_line_to_vec3s(
  const char *obj_line,
  vec3 *v_indices,
  vec3 *vn_indices
);
// void obj_f_uv_n_line_to_vec3s();

#endif