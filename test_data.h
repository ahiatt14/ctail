#ifndef __TAIL_TEST_DATA__
#define __TAIL_TEST_DATA__

#include "tail.h"

vec3 cube_vertex_positions[8] = {
  { 1.000000, 1.000000, -1.000000 }, // 0
  { 1.000000, -1.000000, -1.000000 }, // 1
  { 1.000000, 1.000000, 1.000000 }, // 2
  { 1.000000, -1.000000, 1.000000 }, // 3
  { -1.000000, 1.000000, -1.000000 }, // 4
  { -1.000000, -1.000000, -1.000000 }, // 5
  { -1.000000, 1.000000, 1.000000 }, // 6
  { -1.000000, -1.000000, 1.000000 } // 7
};

unsigned int cube_indices[36] = {
  4, 2, 0,
  2, 7, 3,
  6, 5, 7,
  1, 7, 5,
  0, 3, 1,
  4, 1, 5,
  4, 6, 2,
  2, 6, 7,
  6, 4, 5,
  1, 3, 7,
  0, 2, 3,
  4, 0, 1,
};

#endif