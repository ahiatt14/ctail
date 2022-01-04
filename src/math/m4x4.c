#include <math.h>
#include "m4x4.h"

static inline void fill_m4x4_data(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  float *m
) {
  m[0] = i0; m[4] = i1; m[8] = i2; m[12] = i3;
  m[1] = i4; m[5] = i5; m[9] = i6; m[13] = i7;
  m[2] = i8; m[6] = i9; m[10] = i10; m[14] = i11;
  m[3] = i12; m[7] = i13; m[11] = i14; m[15] = i15;
};

void m4x4_create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  m4x4 *m
) {
  fill_m4x4_data(
    i0, i1, i2, i3,
    i4, i5, i6, i7,
    i8, i9, i10, i11,
    i12, i13, i14, i15,
    m->data
  );
}

void m4x4_x_m4x4(const m4x4 *m0, const m4x4 *m1, m4x4 *dest) {
  for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
      dest->data[c * 4 + r] =
        m0->data[c * 4]     * m1->data[r]     +
        m0->data[c * 4 + 1] * m1->data[4 + r] +
        m0->data[c * 4 + 2] * m1->data[8 + r] +
        m0->data[c * 4 + 3] * m1->data[12 + r];
}

static inline void m4x4_transpose_p(float *m) {
  float temp;
  temp = m[4];
  m[4] = m[1];
  m[1] = temp;
  temp = m[8];
  m[8] = m[2];
  m[2] = temp;
  temp = m[12];
  m[12] = m[3];
  m[3] = temp;
  temp = m[9];
  m[9] = m[6];
  m[6] = temp;
  temp = m[7];
  m[7] = m[13];
  m[13] = temp;
  temp = m[14];
  m[14] = m[11];
  m[11] = temp;
}

void m4x4_transpose(m4x4 *m) {
  m4x4_transpose_p(&(m->data[0]));
}

void m4x4_identity(m4x4 *m) {
  fill_m4x4_data(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4_translation(const vec4 *t, m4x4 *m) {
  fill_m4x4_data(
    1, 0, 0, t->x,
    0, 1, 0, t->y,
    0, 0, 1, t->z,
    0, 0, 0, 1,
    m->data
  );
}

// TODO: could update to scale on each axis if we want idk
void m4x4_scaling(float s, m4x4 *m) {
  fill_m4x4_data(
    s, 0, 0, 0,
    0, s, 0, 0,
    0, 0, s, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4_rotation(float rads, const vec4 *t, m4x4 *m) {

  float c = cos(rads);
  float s = sin(rads);
  float d = 1.0f - c;

  float x = t->x * d;
  float y = t->y * d;
  float z = t->z * d;

  float txty = x * t->y;
  float txtz = x * t->z;
  float tytz = y * t->z;

  fill_m4x4_data(
    c + x * t->x, txty - s * t->z, txtz + s * t->y, 0,
    txty + s * t->z, c + y * t->y, tytz - s * t->x, 0,
    txtz - s * t->y, tytz + s * t->x, c + z * t->z, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4_view(
  const vec4 *right,
  const vec4 *up,
  const vec4 *forward,
  m4x4 *m
) {
  fill_m4x4_data(
    right->x, right->y, right->z, 0,
    up->x, up->y, up->z, 0,
    forward->x, forward->y, forward->z, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4_inverted_translation(
  const vec4 *t,
  m4x4 *m
) {
  fill_m4x4_data(
    1, 0, 0, -t->x,
    0, 1, 0, -t->y,
    0, 0, 1, -t->z,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4_ortho_projection(
  float n,
  float f,
  float l,
  float r,
  float b,
  float t,
  m4x4 *m
) {
  fill_m4x4_data(
    2/(r-l), 0, 0, -(r+l)/(r-l),
    0, 2/(t-b), 0, -(t+b)/(t-b),
    0, 0, -2/(f-n), -(f+n)/(f-n),
    0, 0, 0, 1,
    m->data
  );
}