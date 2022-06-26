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

#define MAX_OUTPUT_PATH_CHAR_COUNT 200

void fprint_vert(FILE *file, const struct vertex *v);
void fprint_vec3(FILE *file, const struct vec3 *t);
void fprint_vec2(FILE *file, const struct vec2 *t);
void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);
void write_header_file(
  const char *filename,
  FILE *file
);
void write_src_file(
  const char *filename,
  struct vertex *vertices,
  unsigned int *indices,
  int vertex_count,
  int index_count,
  FILE *file
);

int main(int argc, char *argv[]) {

  FILE *obj_file = NULL;
  struct vertex vertices[MAX_VERTICES] = {0};
  unsigned int indices[MAX_INDICES] = {0};
  int vertex_count = 0;
  int index_count = 0;

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
  filename_from_path(filename, argv[1], MAX_FILENAME_LENGTH);
  
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

  // TODO: could abstract this file opening stuff
  char header_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcat(header_filepath, argv[3]);
  strcat(header_filepath, filename);
  strcat(header_filepath, "_mesh");
  strcat(header_filepath, ".h");
  FILE *header_file = fopen(header_filepath, "w");
  // TODO: I much prefer putting input validation at the top
  if (!header_file) {
    printf("Could not open %s for writing", header_filepath);
    return 1;
  }
  write_header_file(filename, header_file);
  fclose(header_file);

  // TODO: could abstract this file opening stuff
  char src_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcat(src_filepath, argv[4]);
  strcat(src_filepath, filename);
  strcat(src_filepath, "_mesh");
  strcat(src_filepath, ".c");
  FILE *src_file = fopen(src_filepath, "w");
  // TODO: I much prefer putting input validation at the top
  if (!src_file) {
    printf("Could not open %s for writing", src_filepath);
    return 1;
  }
  write_src_file(
    filename,
    vertices,
    indices,
    vertex_count,
    index_count,
    src_file
  );
  fclose(src_file);

  fclose(obj_file);
  return 0;
}

void fprint_vec3(FILE *file, const struct vec3 *t) {
  fprintf(file, "{ %.6ff, %.6ff, %.6ff }", t->x, t->y, t->z);
}

void fprint_vec2(FILE *file, const struct vec2 *t) {
  fprintf(file, "{ %.6ff, %.6ff }", t->x, t->y);
}

void fprint_vert(FILE *file, const struct vertex *v) {
  fprintf(file, "{");
  fprint_vec3(file, &v->position);
  fprintf(file, ",");
  fprint_vec3(file, &v->normal);
  fprintf(file, ",");
  fprint_vec2(file, &v->uv);
  fprintf(file, "}");
}

void write_header_file(
  const char *filename,
  FILE *file
) {
  fprintf(file, "#ifndef __TAIL_%s_MESH__\n", filename);
  fprintf(file, "#define __TAIL_%s_MESH__\n", filename);
  fprintf(file, "#include \"tail.h\"\n");
  fprintf(file, "extern struct drawable_mesh %s_mesh;\n", filename);
  fprintf(file, "#endif");
}

void write_src_file(
  const char *filename,
  struct vertex *vertices,
  unsigned int *indices,
  int vertex_count,
  int index_count,
  FILE *file
) {
  fprintf(file, "#include \"tail.h\"\n");
  fprintf(file, "#include \"%s_mesh.h\"\n", filename);
  fprintf(file, "struct drawable_mesh %s_mesh = {\n", filename);
  fprintf(file, ".vertices_size = %u,\n", sizeof(struct vertex) * vertex_count);
  fprintf(file, ".indices_size = %u,\n", sizeof(unsigned int) * index_count);
  fprintf(file, ".indices_length = %i,\n", index_count);
  fprintf(file, ".indices = (unsigned int[]){\n");
  for (int i = 0; i < index_count; i+=3) {
    fprintf(file, "%i, %i, %i,\n", indices[i], indices[i+1], indices[i+2]);
  }
  fprintf(file, "},\n");
  fprintf(file, ".vertices = (struct vertex[]){\n");
  for (int i = 0; i < vertex_count; i++){
    fprint_vert(file, &vertices[i]);
    if (i < vertex_count - 1) fprintf(file, ",");
    fprintf(file, "\n");
  }
  fprintf(file, "}};\n");
}

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
) {
  char *last_slash = strrchr(filepath, '/');
  if (last_slash == NULL) last_slash = filepath - 1;
  char *last_dot = strrchr(filepath, '.');
  strncpy(filename_out, last_slash, last_dot - ++last_slash);
}