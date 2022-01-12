#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "public_types.h"

void space__create_model(
  const coordinate_space *space,
  const transform *t,
  m4x4 *model
);

#endif