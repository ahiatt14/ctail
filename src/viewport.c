#include "viewport.h"
#include "public_types.h"
#include "m4x4.h"

const m4x4* viewport__calculate_perspective(
  int near_clip_distance,
  int far_clip_distance,
  int vertical_fov_in_deg,
  viewport *vwprt
) {

  m4x4_create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &vwprt->_perspective
  );

  return &vwprt->_perspective;
}