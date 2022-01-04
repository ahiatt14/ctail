#include <math.h>
#include "vec4.h"
#include "public_types.h"

void vec4_vector(float x, float y, float z, vec4 *t) {
  t->x = x;
  t->y = y;
  t->z = z;
  t->w = 0.0f;
}

void vec4_point(float x, float y, float z, vec4 *t) {
  t->x = x;
  t->y = y;
  t->z = z;
  t->w = 1.0f;
}

void vec4_up(vec4 *t) {
  vec4_vector(0, 1, 0, t);
}

float vec4_magnitude(const vec4 *t) {
  return sqrt(
    t->x * t->x +
    t->y * t->y +
    t->z * t->z +
    t->w * t->w
  );
}

void vec4_normalize(vec4 *t) {
  float m = vec4_magnitude(t);
  t->x = t->x / m;
  t->y = t->y / m;
  t->z = t->z / m;
  t->w = t->w / m;
}

float vec4_dot(const vec4 *t0, const vec4 *t1) {
  return
    t0->x * t1->x +
    t0->y * t1->y +
    t0->z * t1->z;
}

void vec4_cross(const vec4 *t0, const vec4 *t1, vec4 *prod) {
  prod->x = t0->y * t1->z - t0->z * t1->y;
  prod->y = t0->z * t1->x - t0->x * t1->z;
  prod->z = t0->x * t1->y - t0->y * t1->x;
  prod->w = 0.0f; // TODO: is this correct?
}

void vec4_minus_vec4(const vec4 *t0, const vec4 *t1, vec4 *diff) {
  diff->x = t0->x - t1->x;
  diff->y = t0->y - t1->y;
  diff->z = t0->z - t1->z;
  diff->w = t0->w - t1->w;
}

void m4x4_x_vec4(const m4x4 *m, const vec4 *t, vec4 *dest) {
  dest->x = m->data[0] * t->x + m->data[1] * t->y +
    m->data[2] * t->z + m->data[3] * t->w;
  dest->y = m->data[4] * t->x + m->data[5] * t->y +
    m->data[6] * t->z + m->data[7] * t->w;
  dest->z = m->data[8] * t->x + m->data[9] * t->y +
    m->data[10] * t->z + m->data[11] * t->w;
  dest->w = m->data[12] * t->x + m->data[13] * t->y +
    m->data[14] * t->z + m->data[15] * t->w;
}