#ifndef __TAIL_OBJ_PARSER__
#define __TAIL_OBJ_PARSER__

#include <stdlib.h>

#define OBJ_LINE_MAX_LENGTH 100

#include "tail.h"

void print_vert(const struct vertex *v);
void print_vec3(const struct vec3 *t);
void obj_float_line_to_vector(const char *obj_line, float *x);
void obj_f_line_to_3_ui_indices(
  const char *obj_line,
  unsigned int *indices
);
void obj_f_n_line_to_vec3s(
  const char *obj_line,
  struct vec3 *v_indices,
  struct vec3 *vn_indices
);
void parse_obj_into_smooth_mesh(FILE *obj_file);
void parse_obj_into_flat_mesh(FILE *obj_file);
void print_flat_mesh();
void print_smooth_mesh();

#endif