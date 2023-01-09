#include <string.h>
#include <math.h>

#include "vector.h"

struct vec2 vec2_minus_vec2(
  struct vec2 t0,
  struct vec2 t1
) {
  return (struct vec2){
    t0.x - t1.x,
    t0.y - t1.y
  };
}

float vec2__magnitude(
  struct vec2 t
) {
  return sqrt(
    t.x * t.x +
    t.y * t.y
  );
}

struct vec2 vec2__normalize(
  struct vec2 t
) {
  float m = vec2__magnitude(t);
  return (struct vec2){
    t.x / m,
    t.y / m
  };
}

float vec2__dot(
  struct vec2 t0,
  struct vec2 t1
) {
  return
    t0.x * t1.x +
    t0.y * t1.y;
}

float vec2__fewest_rads_btw_vec2s(
  struct vec2 t0,
  struct vec2 t1
) {
  return acos(
    vec2__dot(t0, t1) /
    (vec2__magnitude(t0) * vec2__magnitude(t1))
  );
}

float vec2__dist_from_point_to_line(
  struct vec2 p0,
  struct vec2 p1,
  struct vec2 test_position
) {
  struct vec2 line = vec2_minus_vec2(p1, p0);
  struct vec2 perpendicular = { line.y, -line.x };
  struct vec2 hypotenuse = vec2_minus_vec2(p0, test_position);
  return fabs(vec2__dot(vec2__normalize(perpendicular), hypotenuse));
}

struct vec3 vec3_minus_vec3(
  struct vec3 t0,
  struct vec3 t1
) {
  return (struct vec3){
    t0.x - t1.x,
    t0.y - t1.y,
    t0.z - t1.z
  };
}

struct vec3 vec3_plus_vec3(
  struct vec3 t0,
  struct vec3 t1
) {
  return (struct vec3){
    t0.x + t1.x,
    t0.y + t1.y,
    t0.z + t1.z
  };
}

float vec3__dot(
  struct vec3 t0,
  struct vec3 t1
) {
  return
    t0.x * t1.x +
    t0.y * t1.y +
    t0.z * t1.z;
}

struct vec3 vec3__cross(
  struct vec3 t0,
  struct vec3 t1
) {
  return (struct vec3){
    t0.y * t1.z - t0.z * t1.y,
    t0.z * t1.x - t0.x * t1.z,
    t0.x * t1.y - t0.y * t1.x
  };
}

float vec3__distance(
  struct vec3 t0,
  struct vec3 t1
) {
  return sqrt(
    (t0.x - t1.x) * (t0.x - t1.x) +
    (t0.y - t1.y) * (t0.y - t1.y) +
    (t0.z - t1.z) * (t0.z - t1.z)
  );
}

struct vec3 vec3__mean(
  struct vec3 const *const vec3s,
  int count
) {
  struct vec3 avg = {0};
  for (int component = 0; component < 3; component++) {
    float temp_mean = 0;
    for (int i = 0; i < count; i++) {
      const struct vec3 *current_vec3 = &vec3s[i];
      float v = (&current_vec3->x)[component];
      temp_mean += v;
    }
    (&avg.x)[component] = temp_mean / count;
  }
  return avg;
}

float vec3__magnitude(
  struct vec3 t
) {
  return sqrt(
    t.x * t.x +
    t.y * t.y +
    t.z * t.z
  );
}

struct vec3 vec3__normalize(
  struct vec3 t
) {
  float m = vec3__magnitude(t);
  return (struct vec3){
    t.x / m,
    t.y / m,
    t.z / m
  };
}

struct vec3 vec3__negate(
  struct vec3 t
) {
  return (struct vec3){ -t.x, -t.y, -t.z };
}

struct vec3 scalar_x_vec3(
  float s,
  struct vec3 t
) {
  return (struct vec3){ s * t.x, s * t.y, s * t.z };
}