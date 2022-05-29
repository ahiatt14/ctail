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

  char src[MAX_SRC_CHAR_COUNT] = {0};
  char line[MAX_SRC_LINE_LENGTH] = {0};
  printf("const char *%s_src = \"", filename);
  while(fgets(line, MAX_SRC_LINE_LENGTH, file)) {
    int line_length = strlen(line);
    line[line_length - 1] = '\\';
    line[line_length] = 'n';
    strncat(src, line, line_length + 1);
  }
  // TODO: pretty hacky! could get caught by edge cases of glsl style/formatting
  strcat(src, "}");
  printf("%s", src);
  printf("\";");

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