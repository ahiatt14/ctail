#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "parser.h"

static inline int terminal(char c) {
  return (c == '\0' || c == '\n') ? 1 : 0;
}

void print_vec3(const struct vec3 *t) {
  printf("{ %.6ff, %.6ff, %.6ff }", t->x, t->y, t->z);
}

void print_vert(const struct vertex *v) {
  printf("{");
  print_vec3(&v->position);
  printf(",");
  print_vec3(&v->normal);
  printf("}");
}

void obj_vec3_line_to_vec3(const char *obj_line, struct vec3 *t) {
  int obj_line_index = 0;
  int xyz_offset = 0;
  char *end_of_float;
  while (!terminal(obj_line[obj_line_index])) {
    if (isspace(obj_line[obj_line_index])) {
      (&t->x)[xyz_offset++] =
        strtod(&obj_line[obj_line_index], &end_of_float);
      obj_line_index = end_of_float - obj_line;
    } else obj_line_index++;
  }
}

void obj_f_line_to_3_ui_indices(
  const char *obj_line,
  unsigned int *indices
) {
  int obj_line_index = 0;
  int xyz_offset = 0;
  char *end_of_int;
  while (!terminal(obj_line[obj_line_index])) {
    if (isspace(obj_line[obj_line_index])) {
      indices[xyz_offset++] =
        (unsigned int)strtod(&obj_line[obj_line_index], &end_of_int) - 1;
      obj_line_index = end_of_int - obj_line;
    } else obj_line_index++;
  }
}

void obj_f_n_line_to_vec3s(
  const char *obj_line,
  struct vec3 *v_indices,
  struct vec3 *vn_indices
) {
  int obj_line_index = 0;
  int is_position = 1;
  int v_xyz_offset = 0;
  int vn_xyz_offset = 0;
  char *end_of_int;
  int temp_index = 0;
  while (!terminal(obj_line[obj_line_index])) {
    if (isdigit(obj_line[obj_line_index])) {
      if (is_position) {
        temp_index = (int)strtod(
          &obj_line[obj_line_index],
          &end_of_int) - 1; // .obj indices start at 1
        (&v_indices->x)[v_xyz_offset++] = temp_index;
        is_position = 0;
        obj_line_index = end_of_int - obj_line;
      } else {
        temp_index = (int)strtod(
          &obj_line[obj_line_index],
          &end_of_int) - 1; // .obj indices start at 1
        (&vn_indices->x)[vn_xyz_offset++] = temp_index;
        is_position = 1;
        obj_line_index = end_of_int - obj_line;
      }
    } else obj_line_index++;
  }
}