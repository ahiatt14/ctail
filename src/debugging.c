#include "debugging.h"
#include "gpu.h"
#include "m4x4.h"
#include "vector.h"
#include "camera.h"

#include "space_gizmo_vert.h"
#include "space_gizmo_geo.h"
#include "space_gizmo_frag.h"

static struct shader space_gizmo_shader;

static struct point_buffer space_gizmo_point = {
  .points = (struct vec3[1]){{ 0, 0, 0 }},
  .points_length = 1
};

void debugging__copy_gizmo_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  gpu->copy_points_to_gpu(&space_gizmo_point);

  space_gizmo_shader.vert_src = space_gizmo_vert_src;
  space_gizmo_shader.geo_src = space_gizmo_geo_src;
  space_gizmo_shader.frag_src = space_gizmo_frag_src;
  gpu->copy_shader_to_gpu(&space_gizmo_shader);
}

void debugging__draw_space_gizmo(
  struct gpu_api const *const gpu,
  struct camera const *const cam,
  struct coordinate_space const *const space,
  struct vec3 position
) {
  static struct m4x4 translation;

  gpu->select_shader(&space_gizmo_shader);
  m4x4__translation(position, &translation);

  // TODO: implement UBOs
  gpu->set_shader_m4x4(&space_gizmo_shader, "model", &translation);
  gpu->set_shader_m4x4(&space_gizmo_shader, "view", &cam->lookat);
  gpu->set_shader_m4x4(&space_gizmo_shader, "projection", &cam->projection);
  gpu->set_shader_vec3(&space_gizmo_shader, "up", space->up);
  gpu->set_shader_vec3(&space_gizmo_shader, "right", space->right);
  gpu->set_shader_vec3(&space_gizmo_shader, "forward", space->forward);

  gpu->draw_points(&space_gizmo_point);
}