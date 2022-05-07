#include <stdio.h>
#include <stdlib.h>
#include "glad.h"
#include "gl_util.h"

void report_shader_health(unsigned int id, const char *nickname) {
  GLint success = 0;
  glGetShaderiv((GLuint)id, GL_COMPILE_STATUS, &success);
  if (success != GL_FALSE) {
    printf("Shader %s compiled successfully\n", nickname);
    return;
  }
  GLsizei max_log_size = 0;
  char *log;
  glGetShaderiv((GLuint)id, GL_INFO_LOG_LENGTH, &max_log_size);
  log = (char*)malloc(max_log_size);
  glGetShaderInfoLog(id, max_log_size, NULL, log);
  printf("Compilation of shader %s failed: %s\n", nickname, log);
  free(log);
  return;
}