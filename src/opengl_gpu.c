#include <stdio.h>
#include <stdlib.h>

#include "glad.h"

#include "gpu.h"
#include "vec3.h"
#include "m3x3.h"
#include "m4x4.h"

static const int POSITION_ATTRIB_INDEX = 0;
static const int NORMAL_ATTRIB_INDEX = 1;

static const int COUNT_OF_VALUES_PER_POSITION = 3;
static const int COUNT_OF_VALUES_PER_NORMAL = 3;

static gpu_api gpu;

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

static void copy_program_to_gpu(
  gpu_program *gpup,
  unsigned short int log_compilation_status
) {

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  gpup->_vert_impl_id = vert_id;
  glShaderSource(vert_id, 1, &(gpup->vert_shader_src), NULL);
  glCompileShader(vert_id);
  if (log_compilation_status) report_shader_health((GLint)vert_id, "vert");

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  gpup->_frag_impl_id = frag_id;
  glShaderSource(frag_id, 1, &(gpup->frag_shader_src), NULL);
  glCompileShader(frag_id);
  if (log_compilation_status) report_shader_health((GLint)frag_id, "frag");

  GLuint prog_id = glCreateProgram();
  gpup->_impl_id = prog_id;
  glAttachShader(prog_id, vert_id);
  glAttachShader(prog_id, frag_id);
  glLinkProgram(prog_id);

  glDetachShader(prog_id, vert_id);
  glDetachShader(prog_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
}

void copy_mesh_to_gpu(drawable_mesh *dm) {

  glGenBuffers(1, &dm->_impl_vbo_id);
  glGenBuffers(1, &dm->_impl_ibo_id);
  glGenVertexArrays(1, &dm->_impl_vao_id);

  glBindVertexArray(dm->_impl_vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, dm->_impl_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    dm->vertex_buffer_size,
    &(dm->vertex_buffer->position.x),
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
  glEnableVertexAttribArray(NORMAL_ATTRIB_INDEX);

  glVertexAttribPointer(
    POSITION_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_POSITION,
    GL_FLOAT,
    GL_FALSE,
    sizeof(vertex),
    (GLvoid*)offsetof(vertex, position)
  );

  // TODO: per khronos best practices doc,
  // can optimize normals by packing each
  // normalized into a 32-bit int
  glVertexAttribPointer(
    NORMAL_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_NORMAL,
    GL_FLOAT,
    GL_FALSE,
    sizeof(vertex),
    (GLvoid*)offsetof(vertex, normal)
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dm->_impl_ibo_id);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    dm->index_buffer_size,
    dm->index_buffer,
    GL_STATIC_DRAW
  );
}

void enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

void clear(const float *color) {
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void select_gpu_program(const gpu_program *gpup) {
  glUseProgram(gpup->_impl_id);
}

void set_vertex_shader_m3x3(
  const gpu_program *gpup,
  const char *name,
  const m3x3 *value
) {
  glUniformMatrix3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

void set_vertex_shader_m4x4(
  const gpu_program *gpup,
  const char *name,
  const m4x4 *value
) {
  // TODO: can optimize these by caching locations after
  // first retrieval
  glUniformMatrix4fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

void set_fragment_shader_vec3(
  const gpu_program *gpup,
  const char *name,
  const vec3 *value
) {
  glUniform3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    &value->x
  );
}

void set_fragment_shader_float(
  const gpu_program *gpup,
  const char *name,
  const float value
) {
  glUniform1f(
    glGetUniformLocation(gpup->_impl_id, name),
    value
  );
}

void draw_mesh(const drawable_mesh *mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->index_buffer_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

void cull_back_faces() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void set_viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

int get_viewport_width() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[2];
}

int get_viewport_height() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[3];
}

const gpu_api* gpu__create_api() {

  gpu.clear = clear;
  gpu.enable_depth_test = enable_depth_test;
  gpu.cull_back_faces = cull_back_faces;
  gpu.copy_mesh_to_gpu = copy_mesh_to_gpu;
  gpu.copy_program_to_gpu = copy_program_to_gpu;
  gpu.select_gpu_program = select_gpu_program;
  gpu.set_viewport = set_viewport;
  gpu.get_viewport_width = get_viewport_width;
  gpu.get_viewport_height = get_viewport_height;
  gpu.set_vertex_shader_m3x3 = set_vertex_shader_m3x3;
  gpu.set_vertex_shader_m4x4 = set_vertex_shader_m4x4;
  gpu.set_fragment_shader_vec3 = set_fragment_shader_vec3;
  gpu.set_fragment_shader_float = set_fragment_shader_float;
  gpu.draw_mesh = draw_mesh;

  return &gpu;
}