#include <string.h>
#include <math.h>

#include "camera.h"
#include "viewport.h"
#include "tail_math.h"
#include "m4x4.h"
#include "vector.h"

const M4x4* camera__calculate_perspective(
  Viewport *const vwprt,
  Camera *const cam
) {
  static float r, l, t, b;
  static float m0, m5, m8, m9, m10, m14;

  r = tan(deg_to_rad(cam->horizontal_fov_in_deg)/2) * cam->near_clip_distance;
  l = -r;
  t = r * 1 / viewport__get_aspect_ratio(vwprt);
  b = -t;

  m0 = 2 * cam->near_clip_distance / (r - l);
  m5 = 2 * cam->near_clip_distance / (t - b);
  m8 = (r + l) / (r - l);
  m9 = (t + b) / (t - b);
  m10 =
    -(cam->far_clip_distance + cam->near_clip_distance) /
    (cam->far_clip_distance - cam->near_clip_distance);
  m14 =
    -(2 * cam->far_clip_distance * cam->near_clip_distance) /
    (cam->far_clip_distance - cam->near_clip_distance);

  m4x4__create(
    m0, 0, m8, 0,
    0, m5, m9, 0,
    0, 0, m10, m14,
    0, 0, -1, 0,
    &cam->projection
  );

  return &cam->projection;
}

const M4x4* camera__calculate_lookat(
  Vec3 up,
  Camera *const cam
) {
  static Vec3 camera_forward, camera_right, camera_up;
  static M4x4 view, offset;

  camera_forward = vec3_minus_vec3(cam->position, cam->look_target);
  camera_forward = vec3__normalize(camera_forward);
  camera_right = vec3__cross(up, camera_forward);
  camera_right = vec3__normalize(camera_right);
  camera_up = vec3__cross(camera_forward, camera_right);
  camera_up = vec3__normalize(camera_up);

  m4x4__view(
    camera_right,
    camera_up,
    camera_forward,
    &view
  );
  m4x4__inverted_translation(
    cam->position,
    &offset
  );

  m4x4_x_m4x4(&view, &offset, &cam->lookat);

  return &cam->lookat;
}