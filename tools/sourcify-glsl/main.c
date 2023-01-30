#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_SRC_CHAR_COUNT 10000
#define MAX_SRC_LINE_LENGTH 500

#define MAX_OUTPUT_PATH_CHAR_COUNT 200

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

void str_to_upper(
  char *str
);

int main(int argc, char *argv[]) {

  FILE *glsl_file = fopen(argv[1], "r");

  if (glsl_file == NULL) {
    printf("Shader file not found. Must provide valid glsl filepath.\n");
    return 1;
  }

  char filename[MAX_SRC_LINE_LENGTH] = {0};
  filename_from_path(filename, argv[1], MAX_SRC_LINE_LENGTH);

  char base_output_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcat(base_output_filepath, argv[2]);
  strcat(base_output_filepath, filename);

  char src_output_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcpy(src_output_filepath, base_output_filepath);
  strcat(src_output_filepath, ".c");

  char header_output_filepath[MAX_OUTPUT_PATH_CHAR_COUNT] = {0};
  strcpy(header_output_filepath, base_output_filepath);
  strcat(header_output_filepath, ".h");

  char src_var_name[MAX_SRC_LINE_LENGTH];
  strcpy(src_var_name, filename);
  str_to_upper(src_var_name);
  strcat(src_var_name, "_SRC");

  // HEADER

  FILE *header_file = fopen(header_output_filepath, "w");
  if (!header_file) {
    printf("Could not open %s for writing\n", header_output_filepath);
    return 1;
  }
  fprintf(
    header_file,
    "#ifndef __TAIL_%s__\n"
    "#define __TAIL_%s__\n"
    "extern const char *%s;\n"
    "#endif",
    src_var_name,
    src_var_name,
    src_var_name
  );
  fclose(header_file);

  // SRC

  FILE *src_file = fopen(src_output_filepath, "w");
  if (!src_file) {
    printf("Could not open %s for writing\n", src_output_filepath);
    return 1;
  }

  fprintf(src_file, "#include \"%s.h\"\n", filename);
  fprintf(src_file, "const char *%s = ", src_var_name);

  char line[MAX_SRC_LINE_LENGTH] = {0};
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

void str_to_upper(
  char* str
) {
  int i = 0;
  while (str[i]) {
    str[i] = toupper(str[i]);
    i++;
  }
}