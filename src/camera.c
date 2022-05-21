#include <string.h>
#include <math.h>

#include "camera.h"
#include "viewport.h"
#include "tail_math.h"
#include "m4x4.h"
#include "vec3.h"

// cache for perspective calculation
static float r, l, t, b;
static float m0, m5, m8, m9, m10, m14;

// cache for lookat calculation
static struct vec3 camera_forward, camera_right, camera_up;
static struct m4x4 view, offset;

void camera__init(struct camera *c) {
  m4x4_identity(&c->_lookat);
  m4x4_identity(&c->_perspective);
}

const struct m4x4* camera__get_lookat(const struct camera *c) {
  return &c->_lookat;
}

const struct m4x4* camera__get_perspective(const struct camera *c) {
  return &c->_perspective;
}

const struct vec3* camera__get_position(struct camera *c) {
  return &c->_position;
}

const struct vec3* camera__get_look_target(struct camera *c) {
  return &c->_look_target;
}

void camera__set_position(float x, float y, float z, struct camera *c) {
  c->_position.x = x;
  c->_position.y = y;
  c->_position.z = z;
  c->_lookat_needs_recalculating = 1;
}

void camera__set_horizontal_fov_in_deg(float fov, struct camera *c) {
  c->_horizontal_fov_in_deg = fov;
  c->_perspective_needs_recalculating = 1;
}

void camera__set_near_clip_distance(float n, struct camera *c) {
  c->_near_clip_distance = n;
  c->_perspective_needs_recalculating = 1;
}

void camera__set_far_clip_distance(float f, struct camera *c) {
  c->_far_clip_distance = f;
  c->_perspective_needs_recalculating = 1;
}

float camera__get_horizontal_fov_in_deg(const struct camera *c) {
  return c->_horizontal_fov_in_deg;
}

void camera__set_look_target(const struct vec3 *t, struct camera *c) {
  memcpy(&c->_look_target.x, &t->x, sizeof(struct vec3));
  c->_lookat_needs_recalculating = 1;
}

short int camera__lookat_needs_recalculating(const struct camera *c) {
  return c->_lookat_needs_recalculating;
}

short int camera__perspective_needs_recalculating(const struct camera *c) {
  return c->_perspective_needs_recalculating;
}

const struct m4x4* camera__calculate_perspective(
  struct viewport *vwprt,
  struct camera *cam
) {

  r = tan(deg_to_rad(cam->_horizontal_fov_in_deg)/2) * cam->_near_clip_distance;
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

const struct m4x4* camera__calculate_lookat(
  const struct vec3 *up,
  struct camera *cam
) {

  vec3_minus_vec3(&cam->_position, &cam->_look_target, &camera_forward);
  vec3_normalize(&camera_forward, &camera_forward);
  vec3_cross(up, &camera_forward, &camera_right);
  vec3_normalize(&camera_right, &camera_right);
  vec3_cross(&camera_forward, &camera_right, &camera_up);
  vec3_normalize(&camera_up, &camera_up);

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