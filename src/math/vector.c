#include <string.h>
#include <math.h>

#include "vector.h"
#include "precision.h"

void vec3_minus_vec3(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const diff
) {
  diff->x = t0->x - t1->x;
  diff->y = t0->y - t1->y;
  diff->z = t0->z - t1->z;
}

void vec3_plus_vec3(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const sum
) {
  sum->x = t0->x + t1->x;
  sum->y = t0->y + t1->y;
  sum->z = t0->z + t1->z;
}

void vec3__create(float x, float y, float z, struct vec3 *const t) {
  t->x = x; t->y = y; t->z = z;
}

// TODO: test me!
float vec2__dot(
  struct vec2 const *const t0,
  struct vec2 const *const t1
) {
  return
    t0->x * t1->x +
    t0->y * t1->y;
}

float vec3__dot(
  struct vec3 const *const t0,
  struct vec3 const *const t1
) {
  return
    t0->x * t1->x +
    t0->y * t1->y +
    t0->z * t1->z;
}

void vec3__cross(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const cross
) {
  struct vec3 temp;
  temp.x = t0->y * t1->z - t0->z * t1->y;
  temp.y = t0->z * t1->x - t0->x * t1->z;
  temp.z = t0->x * t1->y - t0->y * t1->x;
  memcpy(&cross->x, &temp.x, sizeof(struct vec3));
}

void vec3__mean(
  struct vec3 const *const vec3s,
  int count,
  struct vec3 *const avg
) {
  for (int component = 0; component < 3; component++) {
    float temp_mean = 0;
    for (int i = 0; i < count; i++) {
      const struct vec3 *current_vec3 = &vec3s[i];
      float v = (&current_vec3->x)[component];
      temp_mean += v;
    }
    (&avg->x)[component] = temp_mean / count;
  }
}

float vec3__magnitude(struct vec3 const *const t) {
  return sqrt(
    t->x * t->x +
    t->y * t->y +
    t->z * t->z
  );
}

float vec2__magnitude(struct vec2 const *const t) {
  return sqrt(
    t->x * t->x +
    t->y * t->y
  );
}

void vec3__normalize(
  struct vec3 const *const t,
  struct vec3 *const normalized
) {
  float m = vec3__magnitude(t);
  normalized->x = t->x / m;
  normalized->y = t->y / m;
  normalized->z = t->z / m;
}

void vec2__normalize(
  struct vec2 const *const t,
  struct vec2 *const normalized
) {
  float m = vec2__magnitude(t);
  normalized->x = t->x / m;
  normalized->y = t->y / m;
}