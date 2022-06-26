#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#define MAX_FILENAME_LENGTH 100
#define MAX_GLSL_CHARS 10000
#define MAX_GLSL_LINE_LENGTH 200

char file_data[MAX_GLSL_CHARS];
char line[MAX_GLSL_LINE_LENGTH];

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

int main(int argc, char *argv[]) {

  FILE *file;
  if ((file = fopen(argv[1], "r"))) {
    while(fgets(line, MAX_GLSL_CHARS, file)) {
      strcat(file_data, line);
    }
    fclose(file);
  } else {
    printf("File not found. First arg must be valid filepath.\n");
    return 1;
  }

  if (!glfwInit()) return 1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow *window = glfwCreateWindow(1, 1, "null", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

  // TODO: somehow able to get past this???
  GLuint id;
  if (strcmp(argv[2], "frag") == 0) {
    id = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (strcmp(argv[2], "vert") == 0) {
    id = glCreateShader(GL_VERTEX_SHADER); 
  } else {
    printf("Second argument must by \"frag\" or \"vert\".\n");
    return 1;
  }

  const char *src = file_data;

  char filename[MAX_FILENAME_LENGTH] = {'\0'};
  filename_from_path(filename, argv[1], MAX_FILENAME_LENGTH);

  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  GLint success = 0;
  glGetShaderiv((GLuint)id, GL_COMPILE_STATUS, &success);
  if (success != GL_FALSE) {
    printf("%s compiled successfully\n", filename);
    return 0;
  }
  GLsizei max_log_size = 0;
  char *log;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_log_size);
  log = (char*)malloc(max_log_size);
  glGetShaderInfoLog(id, max_log_size, NULL, log);
  printf("%s compilation failed: %s\n", filename, log);
  free(log);

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