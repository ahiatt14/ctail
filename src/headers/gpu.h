#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include "drawable_mesh.h"
#include "m4x4.h"
#include "m3x3.h"

typedef struct GPU_PROGRAM {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _impl_id;
  const char *frag_shader_src;
  const char *vert_shader_src;
} gpu_program;

void renderer__clear(const float *color);
void renderer__enable_depth_test();

void renderer__copy_program_to_gpu(
  gpu_program *p,
  short unsigned int log_comilation_status
);
void renderer__copy_mesh_to_gpu(drawable_mesh *dm);
void renderer__select_gpu_program(const gpu_program *p);
void renderer__set_vertex_shader_m3x3(
  const gpu_program *p,
  const char *name,
  const m3x3 *m
);
void renderer__set_vertex_shader_m4x4(
  const gpu_program *p,
  const char *name,
  const m4x4 *m
);
void renderer__set_fragment_shader_vec3(
  const gpu_program *p,
  const char *name,
  const float *value
);
// void renderer__set_fragment_shader_float(
//   const gpu_program *p,
//   const char *name,
//   float value
// );
void renderer__draw_mesh(const drawable_mesh *mesh);

void renderer__cull_back_faces();
void renderer__set_viewport(int x, int y, int width, int height);

int renderer__get_viewport_width();
int renderer__get_viewport_height();

#endif