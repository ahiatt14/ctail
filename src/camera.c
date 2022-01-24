#include <string.h>
#include <math.h>

#include "camera.h"
#include "viewport.h"
#include "public_types.h"
#include "m4x4.h"
#include "vec4.h"

// cache for perspective calculation
static float r, l, t, b;
static float m0, m5, m8, m9, m10, m14;

// cache for lookat calculation
static vec4 camera_forward, camera_right, camera_up;
static m4x4 view, offset;

void camera__init(camera *c) {
  m4x4_identity(&c->_lookat);
}

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

void camera__set_horizontal_fov_in_deg(float fov, camera *c) {
  c->_horizontal_fov_in_deg = fov;
  c->_perspective_needs_recalculating = 1;
}

float camera__get_horizontal_fov_in_deg(const camera *c) {
  return c->_horizontal_fov_in_deg;
}

void camera__set_look_target(const vec4 *t, camera *c) {
  memcpy(&c->_look_target.x, &t->x, sizeof(vec4));
  c->_lookat_needs_recalculating = 1;
}

short int camera__lookat_needs_recalculating(const camera *c) {
  return c->_lookat_needs_recalculating;
}

short int camera__perspective_needs_recalculating(const camera *c) {
  return c->_perspective_needs_recalculating;
}

const m4x4* camera__calculate_perspective(viewport *vwprt, camera *cam) {

  r = tan(cam->_horizontal_fov_in_deg/2) * cam->_near_clip_distance;
  l = -r;
  t = r * 1 / viewport__get_aspect_ratio(vwprt);
  b = -t;

  m0 = 2 * cam->_near_clip_distance / (r - l);
  m5 = 2 * cam->_near_clip_distance / (t - b);
  m8 = (r + l) / (r - l);
  m9 = (t + b) / (t - b);
  m10 =
    -(cam->_far_clip_distance + cam->_near_clip_distance) /
    (cam->_far_clip_distance - cam->_near_clip_distance);
  m14 =
    -(2 * cam->_far_clip_distance * cam->_near_clip_distance) /
    (cam->_far_clip_distance - cam->_near_clip_distance);

  m4x4_create(
    m0, 0, m8, 0,
    0, m5, m9, 0,
    0, 0, m10, m14,
    0, 0, -1, 0,
    &cam->_perspective
  );

  vwprt->_perspective_needs_recalculating = 0;
  cam->_perspective_needs_recalculating = 0;

  return &cam->_perspective;
}

const m4x4* camera__calculate_lookat(
  const vec4 *up,
  camera *cam
) {

  vec4_minus_vec4(&cam->_position, &cam->_look_target, &camera_forward);
  vec4_normalize(&camera_forward);
  vec4_cross(up, &camera_forward, &camera_right);
  vec4_normalize(&camera_right);
  vec4_cross(&camera_forward, &camera_right, &camera_up);
  vec4_normalize(&camera_up);

  m4x4_view(
    &camera_right,
    &camera_up,
    &camera_forward,
    &view
  );
  m4x4_inverted_translation(
    &cam->_position,
    &offset
  );

  m4x4_x_m4x4(&view, &offset, &cam->_lookat);

  cam->_lookat_needs_recalculating = 0;
  return &cam->_lookat;
}