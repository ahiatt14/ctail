#include <math.h>
#include <string.h>

#include "m4x4.h"
#include "vector.h"

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

void m4x4__create(
  float i0, float i1, float i2, float i3,
  float i4, float i5, float i6, float i7,
  float i8, float i9, float i10, float i11,
  float i12, float i13, float i14, float i15,
  M4x4 *const m
) {
  fill_m4x4_data(
    i0, i1, i2, i3,
    i4, i5, i6, i7,
    i8, i9, i10, i11,
    i12, i13, i14, i15,
    m->data
  );
}

void m4x4_x_m4x4(
  M4x4 const *const m0,
  M4x4 const *const m1,
  M4x4 *const dest
) {
  for (int i = 0; i < 16; i++)
    dest->data[i] =
      m0->data[0 +i%4] * m1->data[(int)(floor(i/4)*4+0)] +
      m0->data[4 +i%4] * m1->data[(int)(floor(i/4)*4+1)] +
      m0->data[8 +i%4] * m1->data[(int)(floor(i/4)*4+2)] +
      m0->data[12+i%4] * m1->data[(int)(floor(i/4)*4+3)];
}

static inline void m4x4__transpose_p(float *m) {
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

void m4x4__transpose(M4x4 *const m) {
  m4x4__transpose_p(&(m->data[0]));
}

void m4x4__identity(M4x4 *const m) {
  fill_m4x4_data(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    m->data
  );
}

Vec3 m4x4_x_point(
  M4x4 const *const m,
  Vec3 t
) {
  return (Vec3){
    m->data[0] * t.x + m->data[4] * t.y + m->data[8] * t.z + m->data[12],
    m->data[1] * t.x + m->data[5] * t.y + m->data[9] * t.z + m->data[13],
    m->data[2] * t.x + m->data[6] * t.y + m->data[10] * t.z + m->data[14]
  };
}

void m4x4__translation(
  Vec3 t,
  M4x4 *const m
) {
  fill_m4x4_data(
    1, 0, 0, t.x,
    0, 1, 0, t.y,
    0, 0, 1, t.z,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4__scaling(
  float s,
  M4x4 *const m
) {
  fill_m4x4_data(
    s, 0, 0, 0,
    0, s, 0, 0,
    0, 0, s, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4__rotation(
   float rads,
  Vec3 t,
  M4x4 *const m
) {

  double c = cos(rads);
  double s = sin(rads);
  double d = 1.0f - c;

  double x = t.x * d;
  double y = t.y * d;
  double z = t.z * d;

  double txty = x * t.y;
  double txtz = x * t.z;
  double tytz = y * t.z;

  fill_m4x4_data(
    c + x * t.x, txty - s * t.z, txtz + s * t.y, 0,
    txty + s * t.z, c + y * t.y, tytz - s * t.x, 0,
    txtz - s * t.y, tytz + s * t.x, c + z * t.z, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4__view(
  Vec3 right,
  Vec3 up,
  Vec3 forward,
  M4x4 *const m
) {
  fill_m4x4_data(
    right.x, right.y, right.z, 0,
    up.x, up.y, up.z, 0,
    forward.x, forward.y, forward.z, 0,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4__inverted_translation(
  Vec3 t,
  M4x4 *const m
) {
  fill_m4x4_data(
    1, 0, 0, -t.x,
    0, 1, 0, -t.y,
    0, 0, 1, -t.z,
    0, 0, 0, 1,
    m->data
  );
}

void m4x4__sub3x3_from00(
  M4x4 const *const src,
  M3x3 *const dest
) {
  for (int c = 0; c < 3; c++) for (int r = 0; r < 3; r++)
    dest->data[3 * c + r] = src->data[4 * c + r];
}