#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "parser.h"
#include "normals.h"

#define OBJ_LINE_MAX_LENGTH 100

static inline int terminal(char c) {
  return (c == '\0' || c == '\n') ? 1 : 0;
}

void obj_float_line_to_vector(const char *obj_line, float *x) {
  int obj_line_index = 0;
  int vec_element_offset = 0;
  char *end_of_float;
  while (!terminal(obj_line[obj_line_index])) {
    if (isspace(obj_line[obj_line_index])) {
      x[vec_element_offset++] =
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

void parse_obj_into_smooth_mesh(
  FILE *obj_file,
  struct vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
) {

  // TODO: could clean up these temps

  struct vec3 obj_positions[5000] = {0};
  struct vec2 obj_uvs[5000] = {0};
  char obj_line[OBJ_LINE_MAX_LENGTH] = {0};

  int obj_v_count = 0;
  int obj_uv_count = 0;
  int face_count = 0;
  int index_count = 0;

  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (strncmp(obj_line, "v ", 2) == 0) {
      obj_float_line_to_vector(obj_line, &obj_positions[obj_v_count++].x);
    } else if (strncmp(obj_line, "f ", 2) == 0) {
      obj_f_line_to_3_ui_indices(obj_line, &indices[face_count++ * 3]);
      index_count += 3;
    } else if (strncmp(obj_line, "vt ", 3) == 0) {
      obj_float_line_to_vector(obj_line, &obj_uvs[obj_uv_count++].x);
    }
  }

  struct vec3 temp_normal = {0};
  for (int i = 0; i < obj_v_count; i++) {
    calculate_vertex_normal(
      i,
      face_count * 3,
      indices,
      obj_positions,
      &temp_normal
    );
    memcpy(
      &vertices[i].position.x,
      &obj_positions[i].x,
      sizeof(struct vec3)
    );
    memcpy(
      &vertices[i].normal.x,
      &temp_normal.x,
      sizeof(struct vec3)
    );
    memcpy(
      &vertices[i].uv.x,
      &obj_uvs[i].x,
      sizeof(struct vec2)
    );
  }

  *vert_count_out = obj_v_count;
  *index_count_out = index_count;
}

void parse_obj_into_flat_mesh(
  FILE *obj_file,
  struct vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
) {

  struct vec3 obj_positions[5000] = {0};
  struct vec3 obj_normals[5000] = {0};
  struct vec2 obj_uvs[5000] = {0};

  char obj_line[OBJ_LINE_MAX_LENGTH] = {0};

  struct vec3 temp_vec3a = {0};
  struct vec3 temp_vec3b = {0};
  struct vec2 temp_vec2 = {0};

  int obj_v_count = 0;
  int obj_normal_count = 0;
  int obj_uv_count = 0;
  int vert_count = 0;

  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (strncmp(obj_line, "v ", 2) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec3a.x);
      memcpy(
        &obj_positions[obj_v_count++],
        &temp_vec3a.x,
        sizeof(struct vec3)
      );
    } else if (strncmp(obj_line, "vt ", 3) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec2.x);
      memcpy(
        &obj_uvs[obj_uv_count++],
        &temp_vec2.x,
        sizeof(struct vec2)
      );
    } else if (strncmp(obj_line, "vn", 2) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec3a.x);
      memcpy(
        &obj_normals[obj_normal_count++],
        &temp_vec3a.x,
        sizeof(struct vec3)
      );
    } else if (strncmp(obj_line, "f ", 2) == 0) {
      obj_f_n_line_to_vec3s(obj_line, &temp_vec3a, &temp_vec3b);
      for (int face_vert_index = 0; face_vert_index < 3; face_vert_index++) {
        memcpy(
          &vertices[vert_count].position.x,
          &obj_positions[(int)(&temp_vec3a.x)[face_vert_index]],
          sizeof(struct vec3)
        );
        memcpy(
          &vertices[vert_count].normal.x,
          &obj_normals[(int)(&temp_vec3b.x)[face_vert_index]],
          sizeof(struct vec3)
        );
        vert_count++;
      }
    }
  }

  *vert_count_out = vert_count;
  *index_count_out = vert_count;
  for (int i = 0; i < vert_count; i++) indices[i] = i;
}