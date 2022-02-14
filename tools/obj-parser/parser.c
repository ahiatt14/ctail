#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "parser.h"

void print_vec3(const vec3 *t) {
  printf("{ %.6ff, %.6ff, %.6ff }", t->x, t->y, t->z);
}

void print_vert(const vertex *v) {
  printf("{");
  print_vec3(&v->position);
  printf(",");
  print_vec3(&v->normal);
  printf("}");
}

void obj_vec3_line_to_vec3(const char *obj_line, vec3 *t) {
  int obj_line_index = 0;
  int xyz_offset = 0;
  char *end_of_float;
  while (
    obj_line[obj_line_index] != '\0' &&
    obj_line[obj_line_index] != '\n'
  ) {
    if (isspace(obj_line[obj_line_index])) {
      (&t->x)[xyz_offset++] =
        strtod(&obj_line[obj_line_index], &end_of_float);
      obj_line_index = end_of_float - obj_line;
    } else obj_line_index++;
  }
}

void obj_face_line_to_vec3s(
  const char *obj_line,
  vec3 *v_indices,
  vec3 *vn_indices
) {
  int obj_line_index = 0;
  int is_position = 1;
  int v_xyz_offset = 0;
  int vn_xyz_offset = 0;
  char *end_of_int;
  int temp_index = 0;
  while (
    obj_line[obj_line_index] != '\0' &&
    obj_line[obj_line_index] != '\n'
  ) {
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