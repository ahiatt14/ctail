#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "normals.h"
#include "parser.h"

vec3 obj_positions[5000];
vec3 obj_normals[5000];
unsigned int indices[10000];
vertex vertices[20000];

// TODO: clean this shite up!
char obj_line[OBJ_LINE_MAX_LENGTH];
int obj_position_count = 0;
int obj_normal_count = 0;
int vert_count = 0;
int face_count = 0;
int index_count = 0;
vec3 temp_vec3a = {0};
vec3 temp_vec3b = {0};

FILE *obj_file = NULL;

void parse_obj_into_flat_mesh();
void parse_obj_into_smooth_mesh();
void print_smooth_mesh();
void print_flat_mesh();

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("must include filepath and shading type\n");
    return 1;
  }

  if (
    strcmp(argv[2], "smooth") != 0 &&
    strcmp(argv[2], "flat") != 0
  ) {
    printf(
      "2nd arg must be \"smooth\" or \"flat\"\n"
      "\"%s\" was provided\n", argv[2]
    );
    return 1;
  }

  obj_file = fopen(argv[1], "r");
  if (obj_file == NULL) {
    printf("obj file not found.\n");
    return 1;
  }
  
  if (strcmp(argv[2], "flat") == 0) {
    parse_obj_into_flat_mesh();
    print_flat_mesh();
  }
  if (strcmp(argv[2], "smooth") == 0) {
    parse_obj_into_smooth_mesh();
    print_smooth_mesh();
  }

  fclose(obj_file);
  return 0;
}

void parse_obj_into_flat_mesh() {
  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (strncmp(obj_line, "v ", 2) == 0) {
      obj_vec3_line_to_vec3(obj_line, &temp_vec3a);
      memcpy(
        &obj_positions[obj_position_count++],
        &temp_vec3a.x,
        sizeof(vec3)
      );
    }
    else if (strncmp(obj_line, "vn", 2) == 0) {
      obj_vec3_line_to_vec3(obj_line, &temp_vec3a);
      memcpy(
        &obj_normals[obj_normal_count++],
        &temp_vec3a.x,
        sizeof(vec3)
      );
    }
    else if (strncmp(obj_line, "f ", 2) == 0) {
      obj_f_n_line_to_vec3s(obj_line, &temp_vec3a, &temp_vec3b);
      for (int face_vert_index = 0; face_vert_index < 3; face_vert_index++) {
        memcpy(
          &vertices[vert_count].position.x,
          &obj_positions[(int)(&temp_vec3a.x)[face_vert_index]],
          sizeof(vec3)
        );
        memcpy(
          &vertices[vert_count].normal.x,
          &obj_normals[(int)(&temp_vec3b.x)[face_vert_index]],
          sizeof(vec3)
        );
        vert_count++;
      }
    }
  }
}

void parse_obj_into_smooth_mesh() {
  unsigned int index_trio[3] = {0};
  while (fgets(obj_line, OBJ_LINE_MAX_LENGTH, obj_file) != NULL) {
    if (strncmp(obj_line, "v ", 2) == 0) {
      obj_vec3_line_to_vec3(obj_line, &temp_vec3a);
      memcpy(
        &obj_positions[obj_position_count++],
        &temp_vec3a.x,
        sizeof(vec3)
      );
    }
    else if (strncmp(obj_line, "f ", 2) == 0) {
      obj_f_line_to_3_ui_indices(obj_line, index_trio);
      memcpy(
        &indices[face_count++ * 3],
        index_trio,
        sizeof(unsigned int) * 3
      );
      index_count += 3;
    }
  }
  vec3 temp_normal = {0};
  for (int i = 0; i < obj_position_count; i++) {
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
      sizeof(vec3)
    );
    memcpy(
      &vertices[i].normal.x,
      &temp_normal.x,
      sizeof(vec3)
    );
  }
}

void print_flat_mesh() {
  printf("vertex count: %i\n\n", vert_count);

  printf("{\n");
  for (int i = 0; i < vert_count; i++) {
    printf("\t");
    print_vert(&vertices[i]);
    if (i < vert_count - 1) printf(",");
    printf("\n");
  }
  printf("}");

  printf("\n\n");
  printf("{\n");
  for (int i = 0; i < vert_count; i+=3) {
    printf("\t%i, %i, %i,\n", i, i+1, i+2);
  }
  printf("}");
}

void print_smooth_mesh() {
  printf("vertex count: %i\n\n", obj_position_count);

  printf("{\n");
  for (int i = 0; i < obj_position_count; i++){
    printf("\t");
    print_vert(&vertices[i]);
    if (i < obj_position_count - 1) printf(",");
    printf("\n");
  }
  printf("}");

  printf("\n\n");
  printf("index count: %i\n\n", index_count);
  printf("{\n");
  for (int i = 0; i < index_count; i+=3) {
    printf("\t%i, %i, %i,\n", indices[i], indices[i+1], indices[i+2]);
  }
  printf("}");
}