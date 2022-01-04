#ifndef __TAIL_TYPES__
#define __TAIL_TYPES__

#include <stddef.h>

typedef struct M4X4 {
  float data[16];
} m4x4;

typedef struct VEC4 {
  float x, y, z, w;
} vec4;

typedef struct VIEWPORT {
  int x_pos;
  int y_pos;
  int width;
  int height;
  int has_changed;
  m4x4 projection;
} viewport;

typedef struct VERTEX {
  float position[3];
  float normal[3];
} vertex;

typedef struct TRANSFORM {
  vec4 position;
  vec4 rotation; // TODO: we'll want to represent rotation as quats eventually
  float scale;
  m4x4 model;
} transform;

typedef struct DRAWABLE_MESH {
  vertex *vertex_buffer;
  unsigned int *index_buffer;
  size_t vertex_buffer_size;
  size_t index_buffer_size;
  int index_buffer_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
} drawable_mesh;

typedef struct CAMERA {
  float near_clip_distance;
  float far_clip_distance;
  m4x4 _lookat;
  m4x4 _projection;
  vec4 _position;
  vec4 _look_target;
  short int _updated_since_last_draw;
} camera;

#endif