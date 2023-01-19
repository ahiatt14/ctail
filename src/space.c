#include <string.h>

#include "space.h"
#include "tail_math.h"
#include "quaternion.h"
#include "m4x4.h"
#include "m3x3.h"

struct vec3 space__ccw_angle_rotate(
  struct vec3 axis,
  float radians,
  struct vec3 point
) {
  static struct quaternion q;
  q = quaternion__create(axis, radians);

  return space__ccw_quat_rotate(q, point);
}

struct vec3 space__ccw_quat_rotate(
  struct quaternion q,
  struct vec3 point
) {
  static float b2;
  b2 = q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z;
  
  static struct vec3 pw, bv, cb;
  pw = scalar_x_vec3(q.w * q.w - b2, point);
  bv = scalar_x_vec3(vec3__dot(q.v, point) * 2.0f, q.v);
  cb = scalar_x_vec3(q.w * 2.0f, vec3__cross(q.v, point));

  return vec3_plus_vec3(vec3_plus_vec3(pw, bv), cb);
}

void space__create_model(
  struct coordinate_space const *const space,
  struct transform const *const t,
  struct m4x4 *const dest
) {

  static struct m4x4 scale, translation, rotation, rotate_and_scale;

  m4x4__identity(dest);
  
  m4x4__translation(t->position, &translation);
  m4x4__scaling(t->scale, &scale);  
  quaternion__to_m4x4(t->_rotation, &rotation);

  m4x4_x_m4x4(&rotation, &scale, &rotate_and_scale);
  m4x4_x_m4x4(&translation, &rotate_and_scale, dest);
}

void space__create_normals_model(
  struct m4x4 const *const model,
  struct m3x3 *const dest
) {
  m4x4__sub3x3_from00(model, dest);
  m3x3__inverse(dest, dest);
  m3x3__transpose(dest, dest);
}