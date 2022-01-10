#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "public_types.h"

// const vec4 DEFAULT_UP = { 0.0f, 1.0f, 0.0f, 0.0f };
// const vec4 DEFAULT_RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
// const vec4 DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };

void space__model_transform(
  const coordinate_space space,
  const transform *t,
  m4x4 *model
);

#endif