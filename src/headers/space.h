#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "m4x4.h"

typedef struct COORDINATE_SPACE {
  vec4 up;
  vec4 right;
  vec4 forward;
} coordinate_space;

typedef struct TRANSFORM {
  vec4 position;
  vec4 rotation_in_deg; // TODO: we'll want to represent rotation as quats eventually
  float scale;
} transform;

void space__create_model(
  const coordinate_space *space,
  const transform *t,
  m4x4 *model
);

#endif