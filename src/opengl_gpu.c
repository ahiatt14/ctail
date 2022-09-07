#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "glad.h"

#include "gpu.h"
#include "vector.h"
#include "m3x3.h"
#include "m4x4.h"

#define POSITION_ATTRIB_INDEX 0
#define NORMAL_ATTRIB_INDEX 1
#define UV_ATTRIB_INDEX 2

#define COUNT_OF_VALUES_PER_POSITION 3
#define COUNT_OF_VALUES_PER_NORMAL 3
#define COUNT_OF_VALUES_PER_UV 2

static void compile_src(
  GLenum shader_type,
  const char *shader_src,
  unsigned int *dest_id
) {
  GLuint id = glCreateShader(shader_type);
  *dest_id = id;
  glShaderSource(id, 1, &shader_src, NULL);
  glCompileShader(id);
}

static void copy_shader_to_gpu(
  struct shader *const gpup
) {
  
  compile_src(
    GL_FRAGMENT_SHADER,
    gpup->frag_shader_src,
    &gpup->_frag_impl_id
  );
  compile_src(
    GL_VERTEX_SHADER,
    gpup->vert_shader_src,
    &gpup->_vert_impl_id
  );
  if (gpup->geo_shader_src != NULL)
    compile_src(
      GL_GEOMETRY_SHADER,
      gpup->geo_shader_src,
      &gpup->_geo_impl_id
    );

  GLuint prog_id = glCreateProgram();
  gpup->_impl_id = prog_id;
  glAttachShader(prog_id, gpup->_frag_impl_id);
  glAttachShader(prog_id, gpup->_vert_impl_id);
  if (gpup->geo_shader_src != NULL)
    glAttachShader(prog_id, gpup->_geo_impl_id);
  glLinkProgram(prog_id);

  glDetachShader(prog_id, gpup->_frag_impl_id);
  glDetachShader(prog_id, gpup->_vert_impl_id);
  if (gpup->geo_shader_src != NULL)
    glDetachShader(prog_id, gpup->_geo_impl_id);

  glDeleteShader(gpup->_frag_impl_id);
  glDeleteShader(gpup->_vert_impl_id);
  if (gpup->geo_shader_src != NULL)
    glDeleteShader(gpup->_geo_impl_id);
}

static int channel_count_to_gl_tex_format(int channel_count) {
  switch (channel_count) {
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
  }
  return GL_RGB;
}
static void copy_texture_to_gpu(struct texture *const tex) {

  glGenTextures(1, &tex->_impl_id);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    channel_count_to_gl_tex_format(tex->channels_count),
    tex->width,
    tex->height,
    0, // "should always be 0 (legacy stuff)",
    channel_count_to_gl_tex_format(tex->channels_count),
    GL_UNSIGNED_BYTE,
    tex->data
  );

  glGenerateMipmap(GL_TEXTURE_2D);
}

static void copy_mesh_to_gpu(struct drawable_mesh *const dm, GLenum usage) {

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
static void update_gpu_mesh_data(struct drawable_mesh const *const dm) {
  glBindBuffer(GL_ARRAY_BUFFER, dm->_impl_vbo_id);
  temp_buffer_map = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(
    temp_buffer_map,
    &dm->vertices[0].position.x,
    dm->vertices_size
  );
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

static void copy_static_mesh_to_gpu(struct drawable_mesh *const dm) {
  copy_mesh_to_gpu(dm, GL_STATIC_DRAW);
}

static void copy_dynamic_mesh_to_gpu(struct drawable_mesh *const dm) {
  copy_mesh_to_gpu(dm, GL_DYNAMIC_DRAW);
}

static void enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

static void enable_blending() {
  glEnable(GL_BLEND);
}

static void set_default_blending() {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void enable_MSAA() {
  glEnable(GL_MULTISAMPLE);
}

static void disable_MSAA() {
  glDisable(GL_MULTISAMPLE);
}

static void clear(struct vec3 const *const c) {
  glClearColor(c->x, c->y, c->z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void clear_depth_buffer() {
  glClear(GL_DEPTH_BUFFER_BIT);
}

static void select_texture(struct texture const *const tex) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
}

static void select_textures(
  struct shader const *const shad,
  uint8_t texture_count,
  char const *const *const uniform_names,
  struct texture const *const *const textures
) {
  for (int i = 0; i < texture_count; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    // TODO: inefficient to get uniform locations before every draw call
    glUniform1i(
      glGetUniformLocation(shad->_impl_id, uniform_names[i]),
      i
    );
    glBindTexture(GL_TEXTURE_2D, textures[i]->_impl_id);
  }
}

static void select_shader(struct shader const *const gpup) {
  glUseProgram(gpup->_impl_id);
}

static void set_vertex_shader_m3x3(
  struct shader const *const gpup,
  char const *name,
  struct m3x3 const *const value
) {
  glUniformMatrix3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

static void set_vertex_shader_m4x4(
  struct shader const *const gpup,
  char const *name,
  struct m4x4 const *const value
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

static void set_fragment_shader_vec2(
  struct shader const *const gpup,
  char const *name,
  struct vec2 value
) {
  glUniform2fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    &value.x
  );
}

static void set_fragment_shader_vec3(
  struct shader const *const gpup,
  char const *name,
  struct vec3 value
) {
  glUniform3fv(
    glGetUniformLocation(gpup->_impl_id, name),
    1,
    &value.x
  );
}

static void set_fragment_shader_float(
    struct shader const *const gpup,
    char const *name,
    float value
) {
  glUniform1f(
    glGetUniformLocation(gpup->_impl_id, name),
    value
  );
}

static void draw_mesh(struct drawable_mesh const *const mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

static void draw_wireframe(struct drawable_mesh const *const mesh) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void gpu__create_api(struct gpu_api *const gpu) {
  gpu->clear = clear;
  gpu->clear_depth_buffer = clear_depth_buffer;
  gpu->enable_depth_test = enable_depth_test;
  gpu->enable_blending = enable_blending;
  gpu->set_default_blending = set_default_blending;
  gpu->enable_MSAA = enable_MSAA;
  gpu->disable_MSAA = disable_MSAA;
  gpu->cull_back_faces = cull_back_faces;
  gpu->cull_no_faces = cull_no_faces;
  gpu->copy_static_mesh_to_gpu = copy_static_mesh_to_gpu;
  gpu->copy_dynamic_mesh_to_gpu = copy_dynamic_mesh_to_gpu;
  gpu->update_gpu_mesh_data = update_gpu_mesh_data;
  gpu->copy_texture_to_gpu = copy_texture_to_gpu;
  gpu->copy_shader_to_gpu = copy_shader_to_gpu;
  gpu->select_shader = select_shader;
  gpu->select_texture = select_texture;
  gpu->select_textures = select_textures;
  gpu->set_viewport = set_viewport;
  gpu->get_viewport_width = get_viewport_width;
  gpu->get_viewport_height = get_viewport_height;
  gpu->set_vertex_shader_m3x3 = set_vertex_shader_m3x3;
  gpu->set_vertex_shader_m4x4 = set_vertex_shader_m4x4;
  gpu->set_fragment_shader_vec2 = set_fragment_shader_vec2;
  gpu->set_fragment_shader_vec3 = set_fragment_shader_vec3;
  gpu->set_fragment_shader_float = set_fragment_shader_float;
  gpu->draw_mesh = draw_mesh;
  gpu->draw_wireframe = draw_wireframe;
}