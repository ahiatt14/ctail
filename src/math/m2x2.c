#include "m2x2.h"

void m2x2__create(
  float i0, float i2,
  float i1, float i3,
  M2x2 *const m
) {
  m->data[0] = i0; m->data[2] = i2;
  m->data[1] = i1; m->data[3] = i3;
}

float m2x2__determinant(M2x2 const *const m) {
  return m->data[0] * m->data[3] - m->data[1] * m->data[2];
}