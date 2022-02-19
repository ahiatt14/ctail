#ifndef __TAIL_M4X4__
#define __TAIL_M4X4__

#include "vec4.h"

typedef struct M4X4 {
  float data[16];
} m4x4;

void m4x4_create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  m4x4 *m
);
void m4x4_x_m4x4(const m4x4 *m0, const m4x4 *m1, m4x4 *dest);
void m4x4_transpose(m4x4 *m);
void m4x4_identity(m4x4 *m);
void m4x4_x_vec4(const m4x4 *m, const vec4 *t, vec4 *dest);
void m4x4_translation(const vec4 *t, m4x4 *m);
void m4x4_inverted_translation(const vec4 *t, m4x4 *m);
void m4x4_scaling(float s, m4x4 *m);
void m4x4_rotation(float rads, const vec4 *t, m4x4 *m);
void m4x4_view(
  const vec4 *right,
  const vec4 *up,
  const vec4 *forward,
  m4x4 *m
);

#endif