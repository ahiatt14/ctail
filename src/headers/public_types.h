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
  int _near_clip_distance;
  int _far_clip_distance;
  int _x_pos;
  int _y_pos;
  int _width;
  int _height;
  int _perspective_needs_recalculating;
  m4x4 _perspective;
} viewport;

typedef struct VERTEX {
  float position[3];
  float normal[3];
} vertex;

typedef struct TRANSFORM {
  vec4 position;
  vec4 rotation_in_deg; // TODO: we'll want to represent rotation as quats eventually
  float scale;
} transform;

typedef struct COORDINATE_SPACE {
  vec4 up;
  vec4 right;
  vec4 forward;
} coordinate_space;

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
  m4x4 _lookat;
  vec4 _position;
  vec4 _look_target;
  short int _lookat_needs_recalculating;
} camera;

#endif