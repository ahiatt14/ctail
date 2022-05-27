#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#define MAX_GLSL_CHARS 10000
#define MAX_GLSL_LINE_LENGTH 200

char file_data[MAX_GLSL_CHARS];
char line[MAX_GLSL_LINE_LENGTH];

int main(int argc, char *argv[]) {

  if (!glfwInit()) return 1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow *window = glfwCreateWindow(1, 1, "null", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

  FILE *file;
  if ((file = fopen(argv[1], "r"))) {
    while(fgets(line, MAX_GLSL_CHARS, file)) {
      strcat(file_data, line);
    }
    fclose(file);
  } else {
    printf("File does not exist.\n");
    return 1;
  }

  // TODO: clean up flow here
  GLuint id;
  if (strlen(argv[2]) > 4) {
    printf("Must specify either frag or vert for 2nd param.\n");
    return 1;
  }
  if (strncmp(argv[2], "frag", 4) == 0) {
    id = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (strncmp(argv[2], "vert", 4) == 0) {
    id = glCreateShader(GL_VERTEX_SHADER); 
  } else {
    printf("Must specify either frag or vert for 2nd param.\n");
    return 1;
  }

  const char *src = file_data;
  printf("%s\n\n", src);

  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  GLint success = 0;
  glGetShaderiv((GLuint)id, GL_COMPILE_STATUS, &success);
  if (success != GL_FALSE) {
    printf("Shader compiled successfully\n");
    return 0;
  }
  GLsizei max_log_size = 0;
  char *log;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_log_size);
  log = (char*)malloc(max_log_size);
  glGetShaderInfoLog(id, max_log_size, NULL, log);
  printf("Compilation failed: %s\n", log);
  free(log);

  return 0;
}