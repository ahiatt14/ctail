#include "debugging.h"
#include "gpu.h"
#include "m4x4.h"
#include "vector.h"
#include "camera.h"

#include "space_gizmo_vert.h"
#include "space_gizmo_geo.h"
#include "space_gizmo_frag.h"

static Shader space_gizmo_shader;

static PointBuffer space_gizmo_point = {
  .points = (Vec3[1]){{ 0, 0, 0 }},
  .points_length = 1
};

void debugging__copy_gizmo_assets_to_gpu(
  GPU const *const gpu
) {
  gpu->copy_points_to_gpu(&space_gizmo_point);

  space_gizmo_shader.vert_src = SPACE_GIZMO_VERT_SRC;
  space_gizmo_shader.geo_src = SPACE_GIZMO_GEO_SRC;
  space_gizmo_shader.frag_src = SPACE_GIZMO_FRAG_SRC;
  gpu->copy_shader_to_gpu(&space_gizmo_shader);
}

void debugging__draw_space_gizmo(
  GPU const *const gpu,
  Camera const *const cam,
  CoordinateSpace const *const space,
  Vec3 position
) {
  static M4x4 translation;

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