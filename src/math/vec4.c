#include <math.h>
#include <string.h>

#include "vec4.h"

void vec4_create(float x, float y, float z, float w, vec4 *t) {
  t->x = x;
  t->y = y;
  t->z = z;
  t->w = w;
}

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
  vec4 temp;
  temp.x = t0->y * t1->z - t0->z * t1->y;
  temp.y = t0->z * t1->x - t0->x * t1->z;
  temp.z = t0->x * t1->y - t0->y * t1->x;
  temp.w = 0.0f;
  memcpy(&prod->x, &temp.x, sizeof(vec4));
}

void vec4_minus_vec4(const vec4 *t0, const vec4 *t1, vec4 *diff) {
  diff->x = t0->x - t1->x;
  diff->y = t0->y - t1->y;
  diff->z = t0->z - t1->z;
  diff->w = t0->w - t1->w;
}