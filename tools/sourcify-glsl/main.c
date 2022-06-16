#include <stdio.h>
#include <string.h>

#define MAX_SRC_CHAR_COUNT 10000
#define MAX_SRC_LINE_LENGTH 500

#define MAX_OUTPUT_PATH_CHAR_COUNT 200

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

int main(int argc, char *argv[]) {

  FILE *glsl_file = fopen(argv[1], "r");

  if (glsl_file == NULL) {
    printf("Shader file not found. Must provide valid glsl filepath.\n");
    return 1;
  }

  char filename[100] = {0};
  filename_from_path(filename, argv[1], 100);

  // HEADER
  char header_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcat(header_filepath, argv[2]);
  strcat(header_filepath, filename);
  strcat(header_filepath, ".h");

  FILE *header_file = fopen(header_filepath, "w");
  if (!header_file) {
    printf("Could not open %s for writing\n", header_filepath);
    return 1;
  }
  fprintf(
    header_file,
    "#ifndef __TAIL_%s_SRC__\n"
    "#define __TAIL_%s_SRC__\n"
    "extern const char *%s_src;\n"
    "#endif",
    filename,
    filename,
    filename
  );
  fclose(header_file);

  // SRC
  char src_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcat(src_filepath, argv[3]);
  strcat(src_filepath, filename);
  strcat(src_filepath, ".c");

  FILE *src_file = fopen(src_filepath, "w");
  if (!src_file) {
    printf("Could not open %s for writing\n", src_filepath);
    return 1;
  }

  char line[MAX_SRC_LINE_LENGTH] = {0};
  fprintf(src_file, "const char *%s_src = ", filename);
  while(fgets(line, MAX_SRC_LINE_LENGTH, glsl_file)) {
    char temp_line[MAX_SRC_LINE_LENGTH] = {0};
    strncpy(temp_line, line, strlen(line) - 1);
    fprintf(src_file, "\"%s\\n\"\n", temp_line);
  }
  fprintf(src_file, "\"}\\n\";\n");
  fclose(src_file);

  fclose(glsl_file);
  return 0;
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