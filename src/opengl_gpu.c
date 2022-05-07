#include "glad.h"

#include "gpu.h"
#include "vec3.h"
#include "m3x3.h"
#include "m4x4.h"
#include "gl_util.h"

static const int POSITION_ATTRIB_INDEX = 0;
static const int NORMAL_ATTRIB_INDEX = 1;

static const int COUNT_OF_VALUES_PER_POSITION = 3;
static const int COUNT_OF_VALUES_PER_NORMAL = 3;

void renderer__copy_program_to_gpu(
  gpu_program *p,
  unsigned short int log_compilation_status
) {

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  p->_vert_impl_id = vert_id;
  glShaderSource(vert_id, 1, &(p->vert_shader_src), NULL);
  glCompileShader(vert_id);
  if (log_compilation_status) report_shader_health((GLint)vert_id, "vert");

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  p->_frag_impl_id = frag_id;
  glShaderSource(frag_id, 1, &(p->frag_shader_src), NULL);
  glCompileShader(frag_id);
  if (log_compilation_status) report_shader_health((GLint)frag_id, "frag");

  GLuint prog_id = glCreateProgram();
  p->_impl_id = prog_id;
  glAttachShader(prog_id, vert_id);
  glAttachShader(prog_id, frag_id);
  glLinkProgram(prog_id);

  glDetachShader(prog_id, vert_id);
  glDetachShader(prog_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
}

void renderer__copy_mesh_to_gpu(drawable_mesh *dm) {

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

void renderer__enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

void renderer__clear(const float *color) {
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer__select_gpu_program(const gpu_program *p) {
  glUseProgram(p->_impl_id);
}

void renderer__set_vertex_shader_m3x3(
  const gpu_program *p,
  const char *name,
  const m3x3 *m
) {
  glUniformMatrix3fv(
    glGetUniformLocation(p->_impl_id, name),
    1,
    GL_FALSE,
    &(m->data[0])
  );
}

void renderer__set_vertex_shader_m4x4(
  const gpu_program *p,
  const char *name,
  const m4x4 *m
) {
  // TODO: can optimize these by caching locations after
  // first retrieval
  glUniformMatrix4fv(
    glGetUniformLocation(p->_impl_id, name),
    1,
    GL_FALSE,
    &(m->data[0])
  );
}

void renderer__set_fragment_shader_vec3(
  const gpu_program *p,
  const char *name,
  const float *value
) {
  glUniform3fv(
    glGetUniformLocation(p->_impl_id, name),
    1,
    value
  );
}

void renderer__draw_mesh(const drawable_mesh *mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->index_buffer_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

void renderer__cull_back_faces() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void renderer__set_viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

int renderer__get_viewport_width() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[2];
}

int renderer_get_viewport_height() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[3];
}