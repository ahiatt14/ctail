#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "vector.h"

#include "limits.h"
#include "normals.h"
#include "parser.h"

void fprint_vert(FILE *file, const Vertex *v);
void fprint_vec3(FILE *file, const Vec3 *t);
void fprint_vec2(FILE *file, const Vec2 *t);
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
  const char *src_var_name,
  Vertex *vertices,
  unsigned int *indices,
  int vertex_count,
  int index_count,
  FILE *file
);
void str_to_upper(
  char *str
);

int main(int argc, char *argv[]) {

  FILE *obj_file = NULL;
  Vertex vertices[MAX_VERTICES] = {0};
  unsigned int indices[MAX_INDICES] = {0};
  int vertex_count = 0;
  int index_count = 0;

  obj_file = fopen(argv[1], "r");
  if (obj_file == NULL) {
    printf("obj file not found.\n");
    return 1;
  }

  char filename[MAX_FILENAME_CHAR] = {0};
  filename_from_path(filename, argv[1], MAX_FILENAME_CHAR);
  
  uint8_t flat_shading = obj_shading_is_flat(obj_file);
  fclose(obj_file);
  obj_file = fopen(argv[1], "r");

  // TODO: I think there's a lot of cleanup possible in these fns
  if (flat_shading) {
    parse_obj_into_flat_mesh(
      obj_file,
      vertices,
      indices,
      &vertex_count,
      &index_count
    );
  } else {
    parse_obj_into_smooth_mesh(
      obj_file,
      vertices,
      indices,
      &vertex_count,
      &index_count
    );
  }

  char base_output_filepath[MAX_FILEPATH_CHAR] = {0};
  strcat(base_output_filepath, argv[2]);
  strcat(base_output_filepath, filename);

  char header_output_filepath[MAX_FILEPATH_CHAR] = {0};
  strcat(header_output_filepath, base_output_filepath);
  strcat(header_output_filepath, "_mesh.h");

  char src_output_filepath[200] = {0};
  strcpy(src_output_filepath, base_output_filepath);
  strcat(src_output_filepath, "_mesh.c");

  char src_var_name[200];
  strcpy(src_var_name, filename);
  str_to_upper(src_var_name);
  strcat(src_var_name, "_MESH");

  FILE *header_file = fopen(header_output_filepath, "w");
  if (!header_file) {
    printf("Could not open %s for writing", header_output_filepath);
    return 1;
  }
  write_header_file(src_var_name, header_file);
  fclose(header_file);

  FILE *src_file = fopen(src_output_filepath, "w");
  if (!src_file) {
    printf("Could not open %s for writing", src_output_filepath);
    return 1;
  }
  write_src_file(
    filename,
    src_var_name,
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

void fprint_vec3(FILE *file, const Vec3 *t) {
  fprintf(file, "{ %.6ff, %.6ff, %.6ff }", t->x, t->y, t->z);
}

void fprint_vec2(FILE *file, const Vec2 *t) {
  fprintf(file, "{ %.6ff, %.6ff }", t->x, t->y);
}

void fprint_vert(FILE *file, const Vertex *v) {
  fprintf(file, "{");
  fprint_vec3(file, &v->position);
  fprintf(file, ",");
  fprint_vec3(file, &v->normal);
  fprintf(file, ",");
  fprint_vec2(file, &v->uv);
  fprintf(file, "}");
}

void write_header_file(
  const char *src_var_name,
  FILE *file
) {
  fprintf(file, "#ifndef __TAIL_%s__\n", src_var_name);
  fprintf(file, "#define __TAIL_%s__\n", src_var_name);
  fprintf(file, "#include \"tail.h\"\n");
  fprintf(file, "extern DrawableMesh %s;\n", src_var_name);
  fprintf(file, "#endif");
}

void write_src_file(
  const char *filename,
  const char *src_var_name,
  Vertex *vertices,
  unsigned int *indices,
  int vertex_count,
  int index_count,
  FILE *file
) {
  fprintf(file, "#include \"tail.h\"\n");
  fprintf(file, "#include \"%s_mesh.h\"\n", filename);
  fprintf(file, "DrawableMesh %s = {\n", src_var_name);
  fprintf(file, ".vertices_size = %u,\n", sizeof(Vertex) * vertex_count);
  fprintf(file, ".vertices_length = %u,\n", vertex_count);
  fprintf(file, ".indices_size = %u,\n", sizeof(unsigned int) * index_count);
  fprintf(file, ".indices_length = %i,\n", index_count);
  fprintf(file, ".indices = (unsigned int[]){\n");
  for (int i = 0; i < index_count; i+=3) {
    fprintf(file, "%i, %i, %i,\n", indices[i], indices[i+1], indices[i+2]);
  }
  fprintf(file, "},\n");
  fprintf(file, ".vertices = (Vertex[]){\n");
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

void str_to_upper(
  char* str
) {
  int i = 0;
  while (str[i]) {
    str[i] = toupper(str[i]);
    i++;
  }
}