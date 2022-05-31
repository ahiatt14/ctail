#include <stdio.h>
#include <string.h>

#define MAX_SRC_CHAR_COUNT 10000
#define MAX_SRC_LINE_LENGTH 500

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

int main(int argc, char *argv[]) {

  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    printf("Shader file not found. Must provide valid glsl filepath.\n");
    return 1;
  }

  char filename[100] = {0};
  filename_from_path(filename, argv[1], 100);

  char line[MAX_SRC_LINE_LENGTH] = {0};
  printf("#ifndef __TAIL_%s_SRC__\n", filename);
  printf("#define __TAIL_%s_SRC__\n", filename);
  printf("const char *%s_src = \"\"\n", filename);
  while(fgets(line, MAX_SRC_LINE_LENGTH, file)) {
    char temp_line[MAX_SRC_LINE_LENGTH] = {0};
    strncpy(temp_line, line, strlen(line) - 1);
    printf("\"%s\\n\"\n", temp_line);
  }
  printf("\"}\\n\";\n");
  printf("#endif");
  fclose(file);
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