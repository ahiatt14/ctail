// #include <stdio.h> // TODO: remove?
#include <string.h>
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

static void copy_program_to_gpu(struct gpu_program *gpup) {

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

// TODO: probably just param for # of channels instead 
// of multiple fns like this
static void copy_rgb_texture_to_gpu(struct texture *tex) {
  glGenTextures(1, &tex->_impl_id);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

static void copy_mono_texture_to_gpu(struct texture *tex) {
  glGenTextures(1, &tex->_impl_id);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    tex->width,
    tex->height,
    0, // "should always be 0 (legacy stuff)",
    GL_RED,
    GL_UNSIGNED_BYTE,
    tex->data
  );
}

static void copy_mesh_to_gpu(struct drawable_mesh *dm, GLenum usage) {

  glGenBuffers(1, &dm->_impl_vbo_id);
  glGenBuffers(1, &dm->_impl_ibo_id);
  glGenVertexArrays(1, &dm->_impl_vao_id);

  glBindVertexArray(dm->_impl_vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, dm->_impl_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    dm->vertices_size,
    &(dm->vertices->position.x),
    usage
  );

  glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
  glEnableVertexAttribArray(NORMAL_ATTRIB_INDEX);
  glEnableVertexAttribArray(UV_ATTRIB_INDEX);

  glVertexAttribPointer(
    POSITION_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_POSITION,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct vertex),
    (GLvoid*)offsetof(struct vertex, position)
  );

  // TODO: per khronos best practices doc,
  // can optimize normals by packing each
  // normalized into a 32-bit int
  glVertexAttribPointer(
    NORMAL_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_NORMAL,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct vertex),
    (GLvoid*)offsetof(struct vertex, normal)
  );

  glVertexAttribPointer(
    UV_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_UV,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct vertex),
    (GLvoid*)offsetof(struct vertex, uv)
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dm->_impl_ibo_id);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    dm->indices_size,
    dm->indices,
    GL_STATIC_DRAW
  );
}

void *temp_buffer_map;
static void update_gpu_mesh_data(const struct drawable_mesh *dm) {
  glBindBuffer(GL_ARRAY_BUFFER, dm->_impl_vbo_id);
  temp_buffer_map = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(
    temp_buffer_map,
    &dm->vertices[0].position.x,
    dm->vertices_size
  );
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

static void copy_static_mesh_to_gpu(struct drawable_mesh *dm) {
  copy_mesh_to_gpu(dm, GL_STATIC_DRAW);
}

static void copy_dynamic_mesh_to_gpu(struct drawable_mesh *dm) {
  copy_mesh_to_gpu(dm, GL_DYNAMIC_DRAW);
}

static void enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

static void clear(const float *color) {
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void clear_depth_buffer() {
  glClear(GL_DEPTH_BUFFER_BIT);
}

static void select_texture(const struct texture *tex) {
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
}

static void select_gpu_program(const struct gpu_program *gpup) {
  glUseProgram(gpup->_impl_id);
}

static void set_vertex_shader_m3x3(
  const struct gpu_program *gpup,
  const char *name,
  const struct m3x3 *value
) {
  glUniformMatrix3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

static void set_vertex_shader_m4x4(
  const struct gpu_program *gpup,
  const char *name,
  const struct m4x4 *value
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
  const struct gpu_program *gpup,
  const char *name,
  const struct vec3 *value
) {
  glUniform3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    &value->x
  );
}

static void set_fragment_shader_float(
  const struct gpu_program *gpup,
  const char *name,
  const float value
) {
  glUniform1f(
    glGetUniformLocation(gpup->_impl_id, name),
    value
  );
}

static void draw_mesh(const struct drawable_mesh *mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

static void cull_back_faces() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

static void cull_no_faces() {
  glDisable(GL_CULL_FACE);
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

void gpu__create_api(struct gpu_api *gpu) {
  gpu->clear = clear;
  gpu->clear_depth_buffer = clear_depth_buffer;
  gpu->enable_depth_test = enable_depth_test;
  gpu->cull_back_faces = cull_back_faces;
  gpu->cull_no_faces = cull_no_faces;
  gpu->copy_static_mesh_to_gpu = copy_static_mesh_to_gpu;
  gpu->copy_dynamic_mesh_to_gpu = copy_dynamic_mesh_to_gpu;
  gpu->update_gpu_mesh_data = update_gpu_mesh_data;
  gpu->copy_rgb_texture_to_gpu = copy_rgb_texture_to_gpu;
  gpu->copy_mono_texture_to_gpu = copy_mono_texture_to_gpu;
  gpu->copy_program_to_gpu = copy_program_to_gpu;
  gpu->select_gpu_program = select_gpu_program;
  gpu->select_texture = select_texture;
  gpu->set_viewport = set_viewport;
  gpu->get_viewport_width = get_viewport_width;
  gpu->get_viewport_height = get_viewport_height;
  gpu->set_vertex_shader_m3x3 = set_vertex_shader_m3x3;
  gpu->set_vertex_shader_m4x4 = set_vertex_shader_m4x4;
  gpu->set_fragment_shader_vec3 = set_fragment_shader_vec3;
  gpu->set_fragment_shader_float = set_fragment_shader_float;
  gpu->draw_mesh = draw_mesh;
}