#include <stdio.h>
#include <stdlib.h>

#include "glad.h"

#include "gpu.h"
#include "vec3.h"
#include "m3x3.h"
#include "m4x4.h"

static const int POSITION_ATTRIB_INDEX = 0;
static const int NORMAL_ATTRIB_INDEX = 1;
static const int UV_ATTRIB_INDEX = 2;

static const int COUNT_OF_VALUES_PER_POSITION = 3;
static const int COUNT_OF_VALUES_PER_NORMAL = 3;
static const int COUNT_OF_VALUES_PER_UV = 2;

static gpu_api gpu;

static void copy_program_to_gpu(gpu_program *gpup) {

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  gpup->_vert_impl_id = vert_id;
  glShaderSource(vert_id, 1, &(gpup->vert_shader_src), NULL);
  glCompileShader(vert_id);

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  gpup->_frag_impl_id = frag_id;
  glShaderSource(frag_id, 1, &(gpup->frag_shader_src), NULL);
  glCompileShader(frag_id);

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

static void copy_rgb_texture_to_gpu(texture *tex) {
  glGenTextures(1, &tex->_impl_id);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // TODO: will need to parameterize many of these
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    tex->width,
    tex->height,
    0, // "should always be 0 (legacy stuff)",
    GL_RGB,
    GL_UNSIGNED_BYTE,
    tex->data
  );
}

static void copy_mesh_to_gpu(drawable_mesh *dm) {

  glGenBuffers(1, &dm->_impl_vbo_id);
  glGenBuffers(1, &dm->_impl_ibo_id);
  glGenVertexArrays(1, &dm->_impl_vao_id);

  glBindVertexArray(dm->_impl_vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, dm->_impl_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    dm->vertex_buffer_size,
    &(dm->vertex_buffer->position.x),
    GL_STATIC_DRAW // TODO: this should be parameterized!
  );

  glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
  glEnableVertexAttribArray(NORMAL_ATTRIB_INDEX);
  glEnableVertexAttribArray(UV_ATTRIB_INDEX);

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

  glVertexAttribPointer(
    UV_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_UV,
    GL_FLOAT,
    GL_FALSE,
    sizeof(vertex),
    (GLvoid*)offsetof(vertex, uv)
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dm->_impl_ibo_id);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    dm->index_buffer_size,
    dm->index_buffer,
    GL_STATIC_DRAW
  );
}

static void enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

static void clear(const float *color) {
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void select_texture(const texture *tex) {
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
}

static void select_gpu_program(const gpu_program *gpup) {
  glUseProgram(gpup->_impl_id);
}

static void set_vertex_shader_m3x3(
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

static void set_vertex_shader_m4x4(
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

static void set_fragment_shader_vec3(
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

static void set_fragment_shader_float(
  const gpu_program *gpup,
  const char *name,
  const float value
) {
  glUniform1f(
    glGetUniformLocation(gpup->_impl_id, name),
    value
  );
}

static void draw_mesh(const drawable_mesh *mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->index_buffer_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

static void cull_back_faces() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

static void set_viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

static int get_viewport_width() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[2];
}

static int get_viewport_height() {
  GLint initial_viewport_dimensions[4] = {0};
  glGetIntegerv(GL_VIEWPORT, initial_viewport_dimensions);
  return initial_viewport_dimensions[3];
}

const gpu_api* gpu__create_api() {

  gpu.clear = clear;
  gpu.enable_depth_test = enable_depth_test;
  gpu.cull_back_faces = cull_back_faces;
  gpu.copy_mesh_to_gpu = copy_mesh_to_gpu;
  gpu.copy_rgb_texture_to_gpu = copy_rgb_texture_to_gpu;
  gpu.copy_program_to_gpu = copy_program_to_gpu;
  gpu.select_gpu_program = select_gpu_program;
  gpu.select_texture = select_texture;
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