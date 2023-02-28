#include <string.h>
#include <math.h>

#include "vector.h"

Vec2 vec2_minus_vec2(
  Vec2 t0,
  Vec2 t1
) {
  return (Vec2){
    t0.x - t1.x,
    t0.y - t1.y
  };
}

float vec2__magnitude(
  Vec2 t
) {
  return sqrt(
    t.x * t.x +
    t.y * t.y
  );
}

Vec2 vec2__normalize(
  Vec2 t
) {
  float m = vec2__magnitude(t);
  return (Vec2){
    t.x / m,
    t.y / m
  };
}

float vec2__dot(
  Vec2 t0,
  Vec2 t1
) {
  return
    t0.x * t1.x +
    t0.y * t1.y;
}

float vec2__fewest_rads_btw_vec2s(
  Vec2 t0,
  Vec2 t1
) {
  return acos(
    vec2__dot(t0, t1) /
    (vec2__magnitude(t0) * vec2__magnitude(t1))
  );
}

float vec2__dist_from_point_to_line(
  Vec2 p0,
  Vec2 p1,
  Vec2 test_position
) {
  Vec2 line = vec2_minus_vec2(p1, p0);
  Vec2 perpendicular = { line.y, -line.x };
  Vec2 hypotenuse = vec2_minus_vec2(p0, test_position);
  return fabs(vec2__dot(vec2__normalize(perpendicular), hypotenuse));
}

Vec3 vec3_minus_vec3(
  Vec3 t0,
  Vec3 t1
) {
  return (Vec3){
    t0.x - t1.x,
    t0.y - t1.y,
    t0.z - t1.z
  };
}

Vec3 vec3_plus_vec3(
  Vec3 t0,
  Vec3 t1
) {
  return (Vec3){
    t0.x + t1.x,
    t0.y + t1.y,
    t0.z + t1.z
  };
}

float vec3__dot(
  Vec3 t0,
  Vec3 t1
) {
  return
    t0.x * t1.x +
    t0.y * t1.y +
    t0.z * t1.z;
}

Vec3 vec3__cross(
  Vec3 t0,
  Vec3 t1
) {
  return (Vec3){
    t0.y * t1.z - t0.z * t1.y,
    t0.z * t1.x - t0.x * t1.z,
    t0.x * t1.y - t0.y * t1.x
  };
}

float vec3__distance(
  Vec3 t0,
  Vec3 t1
) {
  return sqrt(
    (t0.x - t1.x) * (t0.x - t1.x) +
    (t0.y - t1.y) * (t0.y - t1.y) +
    (t0.z - t1.z) * (t0.z - t1.z)
  );
}

Vec3 vec3__mean(
  Vec3 const *const vec3s,
  int count
) {
  Vec3 avg = {0};
  for (int component = 0; component < 3; component++) {
    float temp_mean = 0;
    for (int i = 0; i < count; i++) {
      const Vec3 *current_vec3 = &vec3s[i];
      float v = (&current_vec3->x)[component];
      temp_mean += v;
    }
    (&avg.x)[component] = temp_mean / count;
  }
  return avg;
}

float vec3__magnitude(
  Vec3 t
) {
  return sqrt(
    t.x * t.x +
    t.y * t.y +
    t.z * t.z
  );
}

Vec3 vec3__normalize(
  Vec3 t
) {
  float m = vec3__magnitude(t);
  return (Vec3){
    t.x / m,
    t.y / m,
    t.z / m
  };
}

Vec3 vec3__negate(
  Vec3 t
) {
  return (Vec3){ -t.x, -t.y, -t.z };
}

Vec3 scalar_x_vec3(
  float s,
  Vec3 t
) {
  return (Vec3){ s * t.x, s * t.y, s * t.z };
}

Vec2 scalar_x_vec2(
  float s,
  Vec2 t
) {
  return (Vec2){ s * t.x, s * t.y };
}