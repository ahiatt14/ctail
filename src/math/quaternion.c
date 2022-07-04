#include <math.h>
#include "quaternion.h"
#include "m4x4.h"
#include "vec3.h"

// TODO: TEST US

void quaternion__conjugate(
  struct quaternion const *const p,
  struct quaternion *const dest
) {
  dest->v.x = -p->v.x;
  dest->v.y = -p->v.y;
  dest->v.z = -p->v.z;
  dest->w = p->w;
}

void quaternion__create(
  struct vec3 const *const axis,
  float radians,
  struct quaternion *const dest
) {
  dest->w = cos(radians * 0.5f);
  dest->v.x = sin(radians * 0.5f) * axis->x;
  dest->v.y = sin(radians * 0.5f) * axis->y;
  dest->v.z = sin(radians * 0.5f) * axis->z;
}

void quaternion__multiply(
  struct quaternion const *const left_p,
  struct quaternion const *const right_p,
  struct quaternion *const dest
) {
  // TODO: implement me!
}

// TODO: think about static caching
// inject temp memory as param? hmmm
static float x2;
static float y2;
static float z2;
static float xy;
static float xz;
static float yz;
static float wx;
static float wy;
static float wz;
void quaternion__to_m4x4(
  struct quaternion const *const p,
  struct m4x4 *const m
) {
  x2 = p->v.x * p->v.x;
  y2 = p->v.y * p->v.y;
  z2 = p->v.z * p->v.z;
  xy = p->v.x * p->v.y;
  xz = p->v.x * p->v.z;
  yz = p->v.y * p->v.z;
  wx = p->w * p->v.x;
  wy = p->w * p->v.y;
  wz = p->w * p->v.z;
  m4x4__create(
    1 - 2 * (y2 + z2), 2 * (xy - wz), 2 * (xz + wy), 0,
    2 * (xy + wz), 1 - 2 * (x2 + z2), 2 * (yz - wx), 0,
    2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (x2 + y2), 0,
    0, 0, 0, 1,
    m
  );
}