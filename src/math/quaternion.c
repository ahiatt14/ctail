#include <math.h>
#include "quaternion.h"
#include "m4x4.h"
#include "vector.h"

struct Quaternion quaternion__create(
  struct Vec3 axis,
  float radians
) {
  return (struct Quaternion){
    .v = (struct Vec3){
      .x = sin(radians * 0.5f) * axis.x,
      .y = sin(radians * 0.5f) * axis.y,
      .z = sin(radians * 0.5f) * axis.z
    },
    .w = cos(radians * 0.5f)
  };
}

struct Quaternion quaternion__multiply(
  struct Quaternion lp,
  struct Quaternion rp
) {
  return (struct Quaternion){
    (struct Vec3){
      lp.w * rp.v.x + lp.v.x * rp.w + lp.v.y * rp.v.z - lp.v.z * rp.v.y,
      lp.w * rp.v.y - lp.v.x * rp.v.z + lp.v.y * rp.w + lp.v.z * rp.v.x,
      lp.w * rp.v.z + lp.v.x * rp.v.y - lp.v.y * rp.v.x + lp.v.z * rp.w
    },
    lp.w * rp.w - lp.v.x * rp.v.x - lp.v.y * rp.v.y - lp.v.z * rp.v.z
  };
}

void quaternion__to_m4x4(
  struct Quaternion p,
  struct M4x4 *const m
) {

  static float x2, y2, z2, xy, xz, yz, wx, wy, wz;

  x2 = p.v.x * p.v.x;
  y2 = p.v.y * p.v.y;
  z2 = p.v.z * p.v.z;
  xy = p.v.x * p.v.y;
  xz = p.v.x * p.v.z;
  yz = p.v.y * p.v.z;
  wx = p.w * p.v.x;
  wy = p.w * p.v.y;
  wz = p.w * p.v.z;

  m4x4__create(
    1 - 2 * (y2 + z2), 2 * (xy - wz), 2 * (xz + wy), 0,
    2 * (xy + wz), 1 - 2 * (x2 + z2), 2 * (yz - wx), 0,
    2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (x2 + y2), 0,
    0, 0, 0, 1,
    m
  );
}