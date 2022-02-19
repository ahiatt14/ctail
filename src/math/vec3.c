#include <string.h>
#include <math.h>

#include "vec3.h"
#include "precision.h"

void vec3_minus_vec3(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *diff
) {
  diff->x = t0->x - t1->x;
  diff->y = t0->y - t1->y;
  diff->z = t0->z - t1->z;
}

void vec3_plus_vec3(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *sum
) {
  sum->x = t0->x + t1->x;
  sum->y = t0->y + t1->y;
  sum->z = t0->z + t1->z;
}

//TODO: may have to move to tail_math
int vec3_equals_vec3(
  const vec3 *t0,
  const vec3 *t1,
  const float_tolerance *ft
) {
  for (int i = 0; i < 3; i++) {
    if (!ft->within_tolerance(
      (&t0->x)[i],
      (&t1->x)[i],
      ft->tolerance
    )) return 0;
  }
  return 1;
}

void vec3_create(float x, float y, float z, vec3 *t) {
  t->x = x; t->y = y; t->z = z;
}

float vec3_dot(const vec3 *t0, const vec3 *t1) {
  return
    t0->x * t1->x +
    t0->y * t1->y +
    t0->z * t1->z;
}

void vec3_cross(
  const vec3 *t0,
  const vec3 *t1,
  vec3 *cross
) {
  vec3 temp;
  temp.x = t0->y * t1->z - t0->z * t1->y;
  temp.y = t0->z * t1->x - t0->x * t1->z;
  temp.z = t0->x * t1->y - t0->y * t1->x;
  memcpy(&cross->x, &temp.x, sizeof(vec3));
}

void vec3_mean(const vec3 *vec3s, int count, vec3 *avg) {
  for (int component = 0; component < 3; component++) {
    float temp_mean = 0;
    for (int i = 0; i < count; i++) {
      const vec3 *current_vec3 = &vec3s[i];
      float v = (&current_vec3->x)[component];
      temp_mean += v;
    }
    (&avg->x)[component] = temp_mean / count;
  }
}

float vec3_magnitude(const vec3 *t) {
  return sqrt(
    t->x * t->x +
    t->y * t->y +
    t->z * t->z
  );
}

void vec3_normalize(const vec3 *t, vec3 *normalized) {
  float m = vec3_magnitude(t);
  normalized->x = t->x / m;
  normalized->y = t->y / m;
  normalized->z = t->z / m;
}