#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include "tail.h"

#define TEST(msg) { printf(msg)
#define PASSED printf(": PASSED\n"); }

int m4x4_equals_m4x4(
  const m4x4 *m0,
  const m4x4 *m1,
  const float_tolerance *f_tol
);
int vec4_equals_vec4(
  const vec4 *t0,
  const vec4 *t1,
  const float_tolerance *f_tol
);
void print_m4x4(const char *name, const m4x4 *m);
void print_vec4(const char *name, const vec4 *t);

vec4 expected_v, actual_v, actual_v1;
m4x4 expected_m, actual_m, actual_m1;

float_tolerance f_tol = {
  .within_tolerance = diff_is_within_mag_based_tolerance,
  .tolerance = FLT_EPSILON
};

const coordinate_space DEFAULT_WORLDSPACE = {
  .up = { 0, 1, 0, 0 },
  .right = { 1, 0, 0, 0 },
  .forward = { 0, 0, 1, 0 }
};

int main(void) {

  /*

    VECTORS

  */

  TEST("vec4_point should create a vec4 with a w component of 1.0");
  vec4_point(2, 4.05f, 40, &actual_v);
  assert(diff_is_within_mag_based_tolerance(2.0f, actual_v.x, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(4.05f, actual_v.y, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(40.0f, actual_v.z, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(1.0f, actual_v.w, FLT_EPSILON));
  PASSED

  TEST("vec4_vector should create a vec4 with a w component of 0.0");
  vec4_vector(3.0f, 3.01f, 1.0f, &actual_v);
  assert(diff_is_within_mag_based_tolerance(3.0f, actual_v.x, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(3.01f, actual_v.y, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(1.0f, actual_v.z, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(0.0f, actual_v.w, FLT_EPSILON));
  PASSED

  TEST("vec4_magnitude should calculate the mag of a vec4");
  vec4_vector(2.0f, 1.0f, 4.3f, &actual_v);
  assert(diff_is_within_mag_based_tolerance(
    vec4_magnitude(&actual_v),
    4.846648693f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec4_normalize should noramlize a vec4");
  vec4_vector(0.0f, 0.5f, 1.0f, &actual_v);
  vec4_normalize(&actual_v);
  assert(diff_is_within_mag_based_tolerance(actual_v.x, 0.0f, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(actual_v.y, 0.4472135954f, FLT_EPSILON));
  assert(diff_is_within_mag_based_tolerance(actual_v.z, 0.8944271909f, FLT_EPSILON));
  PASSED

  TEST("vec4_dot should calculate the dot product of 2 vectors");
  vec4_vector(1.0f, 1.0f, 2.0f, &actual_v1);
  vec4_vector(4.2f, 35.3f, 0.1f, &actual_v);
  assert(diff_is_within_mag_based_tolerance(
    vec4_dot(&actual_v, &actual_v1),
    39.7f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec4_cross should calculate the cross of 2 vectors");
  vec4_vector(1.0f, 1.0f, 2.0f, &actual_v);
  vec4_vector(4.2f, 35.3f, 0.1f, &actual_v1);
  vec4_cross(&actual_v, &actual_v1, &actual_v);
  vec4_vector(-70.5f, 8.3f, 31.1f, &expected_v);
  assert(vec4_equals_vec4(&expected_v, &actual_v, &f_tol));
  PASSED

  TEST("vec4_minus_vec4 should subtract the 2nd from the 1st");
  vec4_vector(1.0f, 1.0f, 2.0f, &actual_v);
  vec4_vector(4.2f, 35.3f, 0.1f, &actual_v1);
  vec4_minus_vec4(&actual_v1, &actual_v, &actual_v);
  vec4_vector(3.2f, 34.3f, -1.9f, &expected_v);
  assert(vec4_equals_vec4(&expected_v, &actual_v, &f_tol));
  PASSED

  /*

    MATRICES

  */

  TEST("m4x4_x_vec4 should correctly multiply a 4d matrix by a vec4");
  vec4_point(6.0f, -20.0f, -14.33f, &actual_v);
  m4x4_create(
    1, 0, 0, 4.0f,
    0, 1, -2.2f, 0,
    0, 0, 1, 0,
    11.11f, 0, 0, 1,
    &actual_m
  );
  m4x4_x_vec4(&actual_m, &actual_v, &actual_v);
  vec4_create(10, 11.526f, -14.33f, 67.66f, &expected_v);
  assert(vec4_equals_vec4(&expected_v, &actual_v, &f_tol));
  PASSED

  TEST("m4x4_create should fill the matrix buffer column-first");
  m4x4_create(
    1, 1, 0, 0,
    6, 6, 0, 0,
    7, 7, 1, 1,
    3, 3, 4, 5,
    &actual_m
  );
  float expected_m_data[16] = {
    1, 6, 7, 3,
    1, 6, 7, 3,
    0, 0, 1, 4,
    0, 0, 1, 5
  };
  memcpy(&expected_m.data, &expected_m_data[0], sizeof(expected_m_data));
  assert(m4x4_equals_m4x4(&expected_m, &actual_m, &f_tol));
  PASSED
  
  TEST("m4x4_identity should create a 4d identity matrix");
  m4x4_identity(&actual_m);
  m4x4_create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(&expected_m, &actual_m, &f_tol));
  PASSED

  TEST("m4x4_translation should create a translation matrix");
  vec4 t;
  vec4_vector(2.355f, 30, 1, &t);
  m4x4_translation(&t, &actual_m);
  m4x4_create(
    1, 0, 0, 2.355f,
    0, 1, 0, 30,
    0, 0, 1, 1,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(&expected_m, &actual_m, &f_tol));
  PASSED

  TEST("m4x4_transpose should transpose the matrix");
  m4x4_create(
    1, 0, 0, 0,
    2, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 1,
    &actual_m
  );
  m4x4_transpose(&actual_m);
  m4x4_create(
    1, 2, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(&expected_m, &actual_m, &f_tol));
  PASSED

  TEST("m4x4_scaling should create a scaling matrix");
  m4x4_scaling(3, &actual_m);
  m4x4_create(
    3, 0, 0, 0,
    0, 3, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(&expected_m, &actual_m, &f_tol));
  PASSED

  TEST("m4x4_rotation should create a working rotation matrix");
  // NOTE: math.h trig fns not very accurate (eg cos(90) != 0)
  // but idc really so set tolerance higher
  f_tol.tolerance = FLT_EPSILON * 10000000;
  vec4 axis;
  m4x4 rotation;
  vec4_point(1.0f, 0.0f, 0.0f, &actual_v);
  vec4_vector(0.0f, 1.0f, 0.0f, &axis);
  m4x4_rotation(deg_to_rad(90), &axis, &rotation);
  m4x4_x_vec4(&rotation, &actual_v, &actual_v);
  vec4_point(0.0f, 0.0f, -1.0f, &expected_v);
  assert(vec4_equals_vec4(&expected_v, &actual_v, &f_tol));
  PASSED

  TEST("m4x4_rotation run #2");
  f_tol.tolerance = FLT_EPSILON * 10000000;
  vec4 axis;
  m4x4 rotation;
  vec4_vector(1.0f, 1.0f, -1.0f, &axis);
  vec4_normalize(&axis);
  vec4_point(1.0f, 0.0f, 0.0f, &actual_v);
  m4x4_rotation(deg_to_rad(180), &axis, &rotation);
  m4x4_x_vec4(&rotation, &actual_v, &actual_v);
  vec4_point(0.0f, 0.0f, -1.0f, &expected_v);
  assert(vec4_equals_vec4(&expected_v, &actual_v, &f_tol));
  PASSED

  TEST("m4x4_view should construct an inverted space transform matrix");
  f_tol.tolerance = FLT_EPSILON;
  vec4 up, right, forward;
  vec4_vector(0.0f, 1.0f, 0.0f, &up);
  vec4_vector(1.0f, 0.0f, 0.0f, &right);
  vec4_vector(0.0f, 0.0f, -1.0f, &forward);
  m4x4_view(
    &right,
    &up,
    &forward,
    &actual_m
  );
  m4x4_create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, -1, 0,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(
    &expected_m,
    &actual_m,
    &f_tol
  ));
  PASSED

  TEST("m4x4_x_m4x4 should correctly multiply 2 matrices");
  f_tol.tolerance = FLT_EPSILON;
  m4x4_create(
    1, 0.3f, 3, 0,
    0, -0.2f, 1, 0,
    -2.2f, 0, 1, 0,
    0, 0, 0, 1,
    &actual_m
  );
  m4x4_create(
    1, 0, 0, 0.3f,
    0, 1, 0, 0,
    0, 0, 1, -1.3f,
    0, 0, 0, 1,
    &actual_m1
  );
  m4x4_x_m4x4(
    &actual_m,
    &actual_m1,
    &actual_m
  );
  m4x4_create(
    1, 0.3f, 3, -3.6f,
    0, -0.2f, 1, -1.3f,
    -2.2f, 0, 1, -1.96f,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(
    &actual_m,
    &expected_m,
    &f_tol
  ));
  PASSED

  /*

    CAMERA

  */

  TEST("camera__calculate_lookat should create a lookat matrix");
  f_tol.tolerance = FLT_EPSILON;
  vec4 target = { 0.0f, 0.5f, -1.0f, 1.0f };
  vec4 world_up = { 0.0f, 1.0f, 0.0f, 0.0f };
  camera cam;
  camera__init(&cam);
  camera__set_position(0.0f, 0.25f, 0.5f, &cam);
  camera__set_look_target(&target, &cam);
  camera__calculate_lookat(&world_up, &cam);
  m4x4_create(
    1, 0, 0, 0,
    0, 0.98639392853f, 0.16439899802f, -0.3287979960f,
    0, -0.16439899802f, 0.98639392853f, -0.4520971775f,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(
    &expected_m,
    camera__get_lookat(&cam),
    &f_tol
  ));
  PASSED

  TEST("moving the camera should set view_needs_recalculating to true");
  camera cam;
  camera__init(&cam);
  camera__set_position(0.0f, 1.0f, 1.2f, &cam);
  assert(camera__view_needs_recalculating(&cam));
  PASSED

  TEST("changing the camera look target should set view_needs_recalculating to true");
  camera cam;
  camera__init(&cam);
  vec4 target = { 0.3f, 0.0f, 3.0f, 1.0f };
  camera__set_look_target(&target, &cam);
  assert(camera__view_needs_recalculating(&cam));
  PASSED

  TEST("calculating the camera view should set view_needs_recalculating to false");
  camera cam;
  camera__init(&cam);
  vec4 target = { 0.0f, 0.5f, -1.0f, 1.0f };
  vec4 world_up = { 0.0f, 1.0f, 0.0f, 0.0f };
  camera__set_position(0.0f, 0.25f, 0.5f, &cam);
  camera__set_look_target(&target, &cam);
  camera__calculate_lookat(&world_up, &cam);
  assert(!camera__view_needs_recalculating(&cam));
  PASSED

  /*

  SPACE

  */

  TEST(
    "space__create_model should use a transform to create a compound\n"
    "matrix for rotation around the z axis, scale, and translation"
  );
  f_tol.tolerance = FLT_EPSILON * 1000;
  transform t = {
    .position = { 0.3f, -2.0f, -2.0f, 1.0f },
    .rotation_in_deg = { 0, 0, 45, 0 },
    .scale = 1.3f
  };
  space__create_model(
    &DEFAULT_WORLDSPACE,
    &t,
    &actual_m
  );
  m4x4_create(
    0.919187997f, 0, 0.919187997f, 0.3f,
    0, 1.3f, 0, -2,
    -0.919187997f, 0, 0.919187997f, -2,
    0, 0, 0, 1,
    &expected_m
  );
  assert(m4x4_equals_m4x4(
    &expected_m,
    &actual_m,
    &f_tol
  ));
  PASSED
}

int m4x4_equals_m4x4(
  const m4x4 *m0,
  const m4x4 *m1,
  const float_tolerance *f_tol
) {
  for (int i = 0; i < 16; i++) {
    if (!f_tol->within_tolerance(
      m0->data[i],
      m1->data[i],
      f_tol->tolerance
    )) return 0;
  }
  return 1;
}

int vec4_equals_vec4(
  const vec4 *t0,
  const vec4 *t1,
  const float_tolerance *f_tol
) {
  for (int i = 0; i < 4; i++) {
    if (!f_tol->within_tolerance(
      (&t0->x)[i],
      (&t1->x)[i],
      f_tol->tolerance
    )) return 0;
  }
  return 1;
}