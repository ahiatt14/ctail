#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "normals.h"
#include "parser.h"

#define MAX_VERTICES 5000
#define MAX_INDICES 15000
#define MAX_FILENAME_LENGTH 100

void print_vert(const struct vertex *v);
void print_vec3(const struct vec3 *t);
void print_vec2(const struct vec2 *t);
void print_mesh(
  const char *filename,
  struct vertex *vertices,
  unsigned int *indices,
  unsigned int vertex_count,
  unsigned int index_count
);

int main(int argc, char *argv[]) {

  FILE *obj_file = NULL;
  struct vertex vertices[MAX_VERTICES] = {0};
  unsigned int indices[MAX_INDICES] = {0};
  int vertex_count = 0;
  int index_count = 0;

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

  char filename[MAX_FILENAME_LENGTH] = {'\0'};
  char *last_slash = strrchr(argv[1], '/');
  char *last_dot = strrchr(argv[1], '.');
  strncpy(filename, last_slash, last_dot - ++last_slash);
  
  if (strcmp(argv[2], "flat") == 0) {
    parse_obj_into_flat_mesh(
      obj_file,
      vertices,
      indices,
      &vertex_count,
      &index_count
    );
  }
  if (strcmp(argv[2], "smooth") == 0) {
    parse_obj_into_smooth_mesh(
      obj_file,
      vertices,
      indices,
      &vertex_count,
      &index_count
    );
  }

  print_mesh(
    filename,
    vertices,
    indices,
    vertex_count,
    index_count
  );

  fclose(obj_file);
  return 0;
}

void print_vec3(const struct vec3 *t) {
  printf("{ %.6ff, %.6ff, %.6ff }", t->x, t->y, t->z);
}

void print_vec2(const struct vec2 *t) {
  printf("{ %.6ff, %.6ff }", t->x, t->y);
}

void print_vert(const struct vertex *v) {
  printf("{");
  print_vec3(&v->position);
  printf(",");
  print_vec3(&v->normal);
  printf(",");
  print_vec2(&v->uv);
  printf("}");
}

void print_mesh(
  const char *filename,
  struct vertex *vertices,
  unsigned int *indices,
  unsigned int vertex_count,
  unsigned int index_count
) {
  printf("unsigned int %s_vertex_count = %i;\n", filename, vertex_count);

  printf("vertex %s_vertices[%i] = {\n", filename, vertex_count);
  for (int i = 0; i < vertex_count; i++){
    printf("\t");
    print_vert(&vertices[i]);
    if (i < vertex_count - 1) printf(",");
    printf("\n");
  }
  printf("};\n");

  printf("unsigned int %s_index_count = %i;\n", filename, index_count);

  printf("unsigned int %s_indices[%i] = {\n", filename, index_count);
  for (int i = 0; i < index_count; i+=3) {
    printf("\t%i, %i, %i,\n", indices[i], indices[i+1], indices[i+2]);
  }
  printf("};\n");
}