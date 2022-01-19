#ifndef __TAIL_VIEWPORT__
#define __TAIL_VIEWPORT__

#include "public_types.h"

const m4x4* viewport__calculate_perspective(
  int near_clip_distance,
  int far_clip_distance,
  int horizontal_fov_in_deg,
  viewport *vwprt
);

#endif