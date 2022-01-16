#include <string.h>

#include "camera.h"
#include "public_types.h"
#include "m4x4.h"
#include "vec4.h"

void camera__init(camera *c) {
  m4x4_identity(&c->_lookat);
}

const m4x4* camera__calculate_lookat(
  const vec4 *up,
  camera *c
) {

  vec4 camera_forward;
  vec4_minus_vec4(&c->_position, &c->_look_target, &camera_forward);
  vec4_normalize(&camera_forward);
  vec4 camera_right;
  vec4_cross(up, &camera_forward, &camera_right);
  vec4_normalize(&camera_right);
  vec4 camera_up;
  vec4_cross(&camera_forward, &camera_right, &camera_up);
  vec4_normalize(&camera_up);

  m4x4 view;
  m4x4_view(
    &camera_right,
    &camera_up,
    &camera_forward,
    &view
  );
  m4x4 offset;
  m4x4_inverted_translation(
    &c->_position,
    &offset
  );

  m4x4_x_m4x4(&view, &offset, &c->_lookat);

  c->_lookat_needs_recalculating = 0;
  return &c->_lookat;
}

// const m4x4* camera__calculate_ortho_projection(
//   const viewport *vwprt,
//   camera *cmr
// ) {
//   m4x4_ortho_projection(
//     cmr->near_clip_distance,
//     cmr->far_clip_distance,
//     vwprt->x_pos,
//     vwprt->width,
//     vwprt->y_pos,
//     vwprt->height,
//     &cmr->_projection
//   );
//   return &cmr->_projection;
// }

const m4x4* camera__get_lookat(const camera *c) {
  return &c->_lookat;
}

const vec4* camera__get_position(camera *c) {
  return &c->_position;
}

const vec4* camera__get_look_target(camera *c) {
  return &c->_look_target;
}

void camera__set_position(float x, float y, float z, camera *c) {
  c->_position.x = x;
  c->_position.y = y;
  c->_position.z = z;
  c->_position.w = 1.0f;
  c->_lookat_needs_recalculating = 1;
}

void camera__set_look_target(const vec4 *t, camera *c) {
  memcpy(&c->_look_target.x, &t->x, sizeof(vec4));
  c->_lookat_needs_recalculating = 1;
}

int camera__lookat_needs_recalculating(const camera *c) {
  return c->_lookat_needs_recalculating;
}