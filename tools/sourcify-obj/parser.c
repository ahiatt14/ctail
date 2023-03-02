#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "mesh.h"
#include "vector.h"

#include "limits.h"
#include "parser.h"
#include "normals.h"

#define OBJ_LINE_MAX_LENGTH 1000

static inline int terminal(char c) {
  return (c == '\0' || c == '\n') ? 1 : 0;
}

// TODO: real, real messy stuff in here!

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

void obj_f_triplet_line_to_vec3s(
  const char *obj_line,
  float *v_indices,
  float *vn_indices,
  float *vt_indices
) {
  
  char obj_line_copy[OBJ_LINE_MAX_LENGTH] = {0};
  strcpy(obj_line_copy, obj_line);

  char index_triplets[3][10] = {0};
  int index_triplet_index = 0;

  char *token = NULL;
  char *context = NULL;

  strtok_r(obj_line_copy, " ", &context); // toss 1st substr which is just "f"
  while ((token = strtok_r(NULL, " ", &context))) {
    strcpy(index_triplets[index_triplet_index++], token);
  }

  for (int i = 0; i < 3; i++) {

    context = NULL;

    token = strtok_r(index_triplets[i], "/", &context);
    v_indices[i] = strtof(token, NULL) - 1; // - 1 cuz .obj indices are 1-indexed
    token = strtok_r(NULL, "/", &context);
    vt_indices[i] = strtof(token, NULL) - 1;
    token = strtok_r(NULL, "/", &context);
    vn_indices[i] = strtof(token, NULL) - 1;
  }
}

uint8_t obj_shading_is_flat(
  FILE *obj_file
) {

  char obj_line[OBJ_LINE_MAX_LENGTH] = {0};

  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (
      strncmp(obj_line, "s ", 2) == 0 &&
      strncmp(&obj_line[2], "0", 1) == 0
    ) return 1;
  }

  return 0;
}

void parse_obj_into_smooth_mesh(
  FILE *obj_file,
  Vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
) {

  Vec3 obj_positions[MAX_VERTICES] = {0};
  Vec2 obj_uvs[MAX_VERTICES] = {0};
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

  Vec3 temp_normal = {0};
  for (int i = 0; i < obj_v_count; i++) {
    temp_normal = calculate_vertex_normal(
      i,
      face_count * 3,
      indices,
      obj_positions
    );
    vertices[i].position = obj_positions[i];
    vertices[i].normal = temp_normal;
    vertices[i].uv = obj_uvs[i];
  }

  *vert_count_out = obj_v_count;
  *index_count_out = index_count;
}

void parse_obj_into_flat_mesh(
  FILE *obj_file,
  Vertex *vertices,
  unsigned int *indices,
  int *vert_count_out,
  int *index_count_out
) {

  Vec3 obj_positions[MAX_VERTICES] = {0};
  Vec3 obj_normals[MAX_VERTICES] = {0};
  Vec2 obj_uvs[MAX_VERTICES] = {0};

  char obj_line[OBJ_LINE_MAX_LENGTH] = {0};

  Vec3 temp_vec3a = {0};
  Vec3 temp_vec3b = {0};
  Vec3 temp_vec3c = {0};
  Vec2 temp_vec2 = {0};

  int obj_v_count = 0;
  int obj_normal_count = 0;
  int obj_uv_count = 0;
  int vert_count = 0;

  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (strncmp(obj_line, "v ", 2) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec3a.x);
      obj_positions[obj_v_count++] = temp_vec3a;
    } else if (strncmp(obj_line, "vt ", 3) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec2.x);
      obj_uvs[obj_uv_count++] = temp_vec2;
    } else if (strncmp(obj_line, "vn", 2) == 0) {
      obj_float_line_to_vector(obj_line, &temp_vec3b.x);
      obj_normals[obj_normal_count++] = temp_vec3b;
    } else if (strncmp(obj_line, "f ", 2) == 0) {
      obj_f_triplet_line_to_vec3s(
        obj_line,
        &temp_vec3a.x,
        &temp_vec3b.x,
        &temp_vec3c.x
      );
      
      for (int face_vert_index = 0; face_vert_index < 3; face_vert_index++) {

        vertices[vert_count].position =
          obj_positions[(int)(&temp_vec3a.x)[face_vert_index]];
        vertices[vert_count].normal =
          obj_normals[(int)(&temp_vec3b.x)[face_vert_index]];

        if (face_vert_index < 3)
          vertices[vert_count].uv =
            obj_uvs[(int)(&temp_vec3c.x)[face_vert_index]];

        vert_count++;
      }
    }
  }

  *vert_count_out = vert_count;
  *index_count_out = vert_count;
  for (int i = 0; i < vert_count; i++) indices[i] = i;
}