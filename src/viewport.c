#include <math.h>
#include "viewport.h"
#include "public_types.h"
#include "m4x4.h"
#include "tail_math.h"

const m4x4* viewport__calculate_perspective(
  int near_clip_distance,
  int far_clip_distance,
  int horizontal_fov_in_deg,
  viewport *vwprt
) {

  float s = 1 / tan(deg_to_rad(horizontal_fov_in_deg) / 2);
  float m10 =
    -far_clip_distance /
    (far_clip_distance - near_clip_distance);
  float m14 =
    -far_clip_distance * near_clip_distance /
    (far_clip_distance - near_clip_distance);

  m4x4_create(
    s, 0, 0, 0,
    0, s, 0, 0,
    0, 0, m10, m14,
    0, 0, -1, 0,
    &vwprt->_perspective
  );

  return &vwprt->_perspective;
}