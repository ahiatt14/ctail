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

#define UNTESSELLATED 0
#define TESSELLATED 1

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
  Shader *const shader
) {
  
  compile_src(
    GL_FRAGMENT_SHADER,
    shader->frag_src,
    &shader->_frag_impl_id
  );
  compile_src(
    GL_VERTEX_SHADER,
    shader->vert_src,
    &shader->_vert_impl_id
  );
  if (shader->geo_src != NULL)
    compile_src(
      GL_GEOMETRY_SHADER,
      shader->geo_src,
      &shader->_geo_impl_id
    );
  if (shader->tess_ctrl_src != NULL)
    compile_src(
      GL_TESS_CONTROL_SHADER,
      shader->tess_ctrl_src,
      &shader->_tess_ctrl_impl_id
    );
  if (shader->tess_eval_src != NULL)
    compile_src(
      GL_TESS_EVALUATION_SHADER,
      shader->tess_eval_src,
      &shader->_tess_eval_impl_id
    );

  GLuint prog_id = glCreateProgram();
  shader->_impl_id = prog_id;
  glAttachShader(prog_id, shader->_frag_impl_id);
  glAttachShader(prog_id, shader->_vert_impl_id);
  if (shader->geo_src != NULL)
    glAttachShader(prog_id, shader->_geo_impl_id);
  if (shader->tess_ctrl_src != NULL)
    glAttachShader(prog_id, shader->_tess_ctrl_impl_id);
  if (shader->tess_eval_src != NULL)
    glAttachShader(prog_id, shader->_tess_eval_impl_id);
  glLinkProgram(prog_id);

  glDetachShader(prog_id, shader->_frag_impl_id);
  glDeleteShader(shader->_frag_impl_id);

  glDetachShader(prog_id, shader->_vert_impl_id);
  glDeleteShader(shader->_vert_impl_id);

  if (shader->geo_src != NULL) {
    glDetachShader(prog_id, shader->_geo_impl_id);
    glDeleteShader(shader->_geo_impl_id);
  }
  
  if (shader->tess_ctrl_src != NULL) {
    glDetachShader(prog_id, shader->_tess_ctrl_impl_id);
    glDeleteShader(shader->_tess_ctrl_impl_id);
    glDetachShader(prog_id, shader->_tess_eval_impl_id);
    glDeleteShader(shader->_tess_eval_impl_id);   
  }
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

static int texture_filter_to_gl_tex_filter(uint8_t filter) {
  switch (filter) {
    case FILTER__NEAREST:
      return GL_NEAREST;
    case FILTER__LINEAR:
      return GL_LINEAR;
  }
  return FILTER__NEAREST;
}

static int texture_wrap_to_gl_tex_wrap(uint8_t wrap) {
  switch (wrap) {
    case WRAP__REPEAT:
      return GL_REPEAT;
    case WRAP__MIRROR_REPEAT:
      return GL_MIRRORED_REPEAT;
    case WRAP__CLAMP:
      return GL_CLAMP_TO_EDGE;
  }
  return GL_REPEAT;
}

static void copy_texture_to_gpu(
  uint8_t filter,
  uint8_t wrap,
  Texture *const tex
) {

  glGenTextures(1, &tex->_impl_id);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_S,
    texture_wrap_to_gl_tex_wrap(wrap)
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_T,
    texture_wrap_to_gl_tex_wrap(wrap)
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    texture_filter_to_gl_tex_filter(filter)
  );

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    channel_count_to_gl_tex_format(tex->channel_count),
    tex->width,
    tex->height,
    0, // "should always be 0 (legacy stuff)",
    channel_count_to_gl_tex_format(tex->channel_count),
    GL_UNSIGNED_BYTE,
    tex->data
  );

  glGenerateMipmap(GL_TEXTURE_2D);
}

static void copy_cubemap_to_gpu(
  uint8_t filter,
  Cubemap *const cubemap
) {

  glGenTextures(1, &cubemap->_impl_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->_impl_id);

  for (unsigned int i = 0; i < 6; i++) {
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
      0,
      GL_RGB,
      cubemap->sides[i]->width,
      cubemap->sides[i]->height,
      0, // "should always be 0 (legacy stuff)",
      GL_RGB,
      GL_UNSIGNED_BYTE,
      cubemap->sides[i]->data
    );
  }

  glTexParameteri(
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_MIN_FILTER,
    texture_filter_to_gl_tex_filter(filter)
  );
  glTexParameteri(
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_MAG_FILTER,
    texture_filter_to_gl_tex_filter(filter)
  );
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

static void copy_points_to_gpu(
  PointBuffer *const pb
) {
  glGenBuffers(1, &pb->_impl_vbo_id);
  glGenVertexArrays(1, &pb->_impl_vao_id);

  glBindVertexArray(pb->_impl_vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, pb->_impl_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(Vec3) * pb->points_length,
    &(pb->points->x),
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
  glVertexAttribPointer(
    POSITION_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_POSITION,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vec3),
    (GLvoid*)offsetof(Vec3, x)
  );
}

static void copy_mesh_to_gpu(
  DrawableMesh *const mesh,
  GLenum usage,
  uint8_t tessellated
) {

  glGenBuffers(1, &mesh->_impl_vbo_id);
  glGenBuffers(1, &mesh->_impl_ibo_id);
  glGenVertexArrays(1, &mesh->_impl_vao_id);

  glBindVertexArray(mesh->_impl_vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->_impl_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    mesh->vertices_size,
    &(mesh->vertices->position.x),
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
    sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, position)
  );

  glVertexAttribPointer(
    NORMAL_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_NORMAL,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, normal)
  );

  glVertexAttribPointer(
    UV_ATTRIB_INDEX,
    COUNT_OF_VALUES_PER_UV,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, uv)
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->_impl_ibo_id);
  // NOTE: quad patches only for now
  if (tessellated) glPatchParameteri(GL_PATCH_VERTICES, 4);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    mesh->indices_size,
    mesh->indices,
    GL_STATIC_DRAW
  );
}

static void update_gpu_mesh_data(DrawableMesh const *const mesh) {
  glBindBuffer(GL_ARRAY_BUFFER, mesh->_impl_vbo_id);
  static void *temp_buffer_map;
  temp_buffer_map = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(
    temp_buffer_map,
    &mesh->vertices[0].position.x,
    mesh->vertices_size
  );
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

// TODO: switch to single copy_mesh_to_gpu fn
// with extra parameters now that there's more than one dimension
static void copy_static_mesh_to_gpu(DrawableMesh *const mesh) {
  copy_mesh_to_gpu(mesh, GL_STATIC_DRAW, UNTESSELLATED);
}

static void copy_dynamic_mesh_to_gpu(DrawableMesh *const mesh) {
  copy_mesh_to_gpu(mesh, GL_DYNAMIC_DRAW, UNTESSELLATED);
}

static void copy_tessellated_mesh_to_gpu(DrawableMesh *const mesh) {
  copy_mesh_to_gpu(mesh, GL_STATIC_DRAW, TESSELLATED);
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

static void clear(Vec3 const *const c) {
  glClearColor(c->x, c->y, c->z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void clear_depth_buffer() {
  glClear(GL_DEPTH_BUFFER_BIT);
}

static void select_cubemap(Cubemap const *const cubemap) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->_impl_id);
}

static void select_texture(Texture const *const tex) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex->_impl_id);
}

static void select_textures(
  Shader const *const shad,
  uint8_t texture_count,
  Texture const *const *const textures
) {
  for (int i = 0; i < texture_count; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glUniform1i(
      glGetUniformLocation(shad->_impl_id, textures[i]->name),
      i
    );
    glBindTexture(GL_TEXTURE_2D, textures[i]->_impl_id);
  }
}

static void select_shader(Shader const *const shader) {
  glUseProgram(shader->_impl_id);
}

static void set_shader_m3x3(
  Shader const *const shader,
  char const *name,
  M3x3 const *const value
) {
  glUniformMatrix3fv(
    glGetUniformLocation(shader->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

static void set_shader_m4x4(
  Shader const *const shader,
  char const *name,
  M4x4 const *const value
) {
  glUniformMatrix4fv(
    glGetUniformLocation(shader->_impl_id, name),
    1,
    GL_FALSE,
    &(value->data[0])
  );
}

static void set_shader_vec2(
  Shader const *const shader,
  char const *name,
  Vec2 value
) {
  glUniform2fv(
    glGetUniformLocation(shader->_impl_id, name),
    1,
    &value.x
  );
}

static void set_shader_vec3(
  Shader const *const shader,
  char const *name,
  Vec3 value
) {
  glUniform3fv(
    glGetUniformLocation(shader->_impl_id, name),
    1,
    &value.x
  );
}

static void set_shader_float(
    Shader const *const shader,
    char const *name,
    float value
) {
  glUniform1f(
    glGetUniformLocation(shader->_impl_id, name),
    value
  );
}

static void draw_mesh(DrawableMesh const *const mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_TRIANGLES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    (GLvoid*)0
  );
}

static void draw_tessellated_mesh(DrawableMesh const *const mesh) {
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_PATCHES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    0
  );
}

static void draw_tessellated_wireframe(DrawableMesh const *const mesh) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(mesh->_impl_vao_id);
  glDrawElements(
    GL_PATCHES,
    mesh->indices_length,
    GL_UNSIGNED_INT,
    0
  );
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void draw_wireframe(DrawableMesh const *const mesh) {
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

static void draw_points(PointBuffer const *const pb) {
  glBindVertexArray(pb->_impl_vao_id);
  glDrawArrays(
    GL_POINTS,
    0,
    pb->points_length
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

void gpu__create_api(GPU *const gpu) {

  glEnable(GL_PROGRAM_POINT_SIZE);

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
  gpu->copy_tessellated_mesh_to_gpu = copy_tessellated_mesh_to_gpu;
  gpu->copy_points_to_gpu = copy_points_to_gpu;
  gpu->update_gpu_mesh_data = update_gpu_mesh_data;
  gpu->copy_texture_to_gpu = copy_texture_to_gpu;
  gpu->copy_cubemap_to_gpu = copy_cubemap_to_gpu;
  gpu->copy_shader_to_gpu = copy_shader_to_gpu;
  gpu->select_shader = select_shader;
  gpu->select_cubemap = select_cubemap;
  gpu->select_texture = select_texture;
  gpu->select_textures = select_textures;
  gpu->set_viewport = set_viewport;
  gpu->get_viewport_width = get_viewport_width;
  gpu->get_viewport_height = get_viewport_height;
  gpu->set_shader_m3x3 = set_shader_m3x3;
  gpu->set_shader_m4x4 = set_shader_m4x4;
  gpu->set_shader_vec2 = set_shader_vec2;
  gpu->set_shader_vec3 = set_shader_vec3;
  gpu->set_shader_float = set_shader_float;
  gpu->draw_mesh = draw_mesh;
  gpu->draw_tessellated_mesh = draw_tessellated_mesh;
  gpu->draw_tessellated_wireframe = draw_tessellated_wireframe;
  gpu->draw_wireframe = draw_wireframe;
  gpu->draw_points = draw_points;
}