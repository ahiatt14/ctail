#include <math.h>
#include <string.h>

#include "tail_math.h"
#include "m3x3.h"
#include "m2x2.h"

static inline int m2x2_r_c_to_index(int r, int c) {
  return 2 * c + r;
}

static inline int m3x3_r_c_to_index(int r, int c) {
  return 3 * c + r;
}

void m3x3__create(
  float i0, float i3, float i6,
  float i1, float i4, float i7,
  float i2, float i5, float i8,
  struct M3x3 *const m
) {
  m->data[0] = i0; m->data[3] = i3; m->data[6] = i6;
  m->data[1] = i1; m->data[4] = i4; m->data[7] = i7;
  m->data[2] = i2; m->data[5] = i5; m->data[8] = i8;
}

void m3x3__identity(struct M3x3 *const m) {
  m3x3__create(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    m
  );
}

float m3x3__minor(int r, int c, struct M3x3 const *const m) {
  struct M2x2 sub_cache = {0};
  int sub_indices[4] = {0};
  int sub_index = 0;
  for (int sub_c = 0; sub_c < 3; sub_c++) {
    for (int sub_r = 0; sub_r < 3; sub_r++) {
      if (sub_c == c || sub_r == r) continue;
      sub_indices[sub_index++] = m3x3_r_c_to_index(sub_r, sub_c);
    }
  }
  for (int i = 0; i < 4; i++) {
    sub_cache.data[i] = m->data[sub_indices[i]];
  }
  return m2x2__determinant(&sub_cache);
}

void m3x3__minors(struct M3x3 const *const src, struct M3x3 *const dest) {
  struct M3x3 temp = {0};
  for (int c = 0; c < 3; c++) {
    for (int r = 0; r < 3; r++) {
      float minor = m3x3__minor(r, c, src);
      temp.data[m3x3_r_c_to_index(r, c)] = minor;
    }
  }
  memcpy(&dest->data[0], &temp.data[0], sizeof(struct M3x3));
}

void m3x3__cofactors(struct M3x3 const *const src, struct M3x3 *const dest) {
  for (int i = 0; i < 9; i++)
    dest->data[i] = (i % 2 == 0) ? src->data[i] : -src->data[i];
}

void m3x3__transpose(struct M3x3 const *const src, struct M3x3 *const dest) {
  float temp;
  dest->data[0] = src->data[0];
  dest->data[4] = src->data[4];
  dest->data[8] = src->data[8];
  temp = src->data[1];
  dest->data[1] = src->data[3];
  dest->data[3] = temp;
  temp = src->data[2];
  dest->data[2] = src->data[6];
  dest->data[6] = temp;
  temp = src->data[5];
  dest->data[5] = src->data[7];
  dest->data[7] = temp;
}

float m3x3__determinant(struct M3x3 const *const m) {
  return
    m->data[0]*(m->data[4]*m->data[8]-m->data[5]*m->data[7]) -
    m->data[3]*(m->data[1]*m->data[8]-m->data[2]*m->data[7]) +
    m->data[6]*(m->data[1]*m->data[5]-m->data[2]*m->data[4]);
}

static void float_x_m3x3(float s, const struct M3x3 *src, struct M3x3 *dest) {
  for (int i = 0; i < 9; i++) dest->data[i] = s * src->data[i];
}

void m3x3__inverse(struct M3x3 const *const src, struct M3x3 *const dest) {
  struct M3x3 minors = {0};
  struct M3x3 cofactors = {0};
  struct M3x3 transposed = {0};
  m3x3__minors(src, &minors);
  m3x3__cofactors(&minors, &cofactors);
  m3x3__transpose(&cofactors, &transposed);
  float_x_m3x3(
    1/m3x3__determinant(src),
    &transposed,
    dest
  );
}