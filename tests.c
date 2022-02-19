#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "tail.h"
#include "test_data.h"

#define TEST(msg) { printf(msg)
#define PASSED printf(": PASSED\n"); }
#define QUIT return 0;

int m2x2_equals_m2x2(
  const m2x2 *m0,
  const m2x2 *m1,
  const float_tolerance *ft
);
int m3x3_equals_m3x3(
  const m3x3 *m0,
  const m3x3 *m1,
  const float_tolerance *ft
);
int m4x4_equals_m4x4(
  const m4x4 *m0,
  const m4x4 *m1,
  const float_tolerance *ft
);
void print_m3x3(const char *name, const m3x3 *m);
void print_m4x4(const char *name, const m4x4 *m);
void print_vec3(const char *name, const vec3 *t);

vec3 expected_v3, actual_v3, actual_v3b, actual_v3c, actual_v3d;
m2x2 expected_m2, actual_m2;
m3x3 expected_m3, actual_m3;
m4x4 expected_m4, actual_m4, actual_m4b;

float_tolerance f_tol = {
  .within_tolerance = diff_is_within_tolerance,
  .tolerance = FLT_EPSILON
};

const coordinate_space DEFAULT_WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, 1 }
};

int main(void) {

  /*

    MATH

  */

  TEST("iclamp should return 3 when clamping 5 between 0 and 3");
  assert(iclamp(5, 0, 3) == 3);
  PASSED

  TEST("iclamp should return 1 when clamping 2 between 0 and 1");
  assert(iclamp(2, 0, 1) == 1);
  PASSED

  TEST(
    "calculate_face_normal should calculate a normalized\n"
    "normal for the provided triangle of positions"
  );
  f_tol.tolerance = FLT_EPSILON;
  vec3 positions[3] = {
    { -1, 0, 0 },
    { 0, 0, 1 },
    { 0, 1, 0 }
  };
  calculate_face_normal(positions, &actual_v3);
  vec3_create(
    -0.577350269f,
    0.577350269f,
    0.577350269f,
    &expected_v3
  );
  PASSED

  TEST("calculate_face_normal run #2");
  f_tol.tolerance = FLT_EPSILON;
  vec3 positions[3] = {
    { 0, 0, 0 },
    { 0, 0, -1 },
    { 0, 1, 0 }
  };
  calculate_face_normal(positions, &actual_v3);
  vec3_create(1, 0, 0, &expected_v3);
  PASSED

  TEST(
    "copy_vec3s should copy the vec3s values for the given indices\n"
    "to the provided buffer"
  );
  f_tol.tolerance = FLT_EPSILON;
  vec3 vec3s[7] = {
    { 0, 0, 0 },
    { 1, 1, 1 },
    { 2, 2, 2 },
    { 3, 3, 3 },
    { 4, 4, 4 },
    { 5, 5, 5 },
    { 6, 6, 6 }
  };
  vec3 actual_v3s[3] = {0};
  unsigned int indices[3] = { 2, 4, 5 };
  vec3 expected_v3s[3] = {
    { 2, 2, 2 },
    { 4, 4, 4 },
    { 5, 5, 5 }
  };
  copy_vec3s(
    vec3s,
    indices,
    3,
    actual_v3s
  );
  for (int i = 0; i < 3; i++) {
    assert(vec3_equals_vec3(
      &actual_v3s[i],
      &expected_v3s[i],
      &f_tol
    ));
  }
  PASSED

  TEST(
    "calculate_vertex_normal should calculate a\n"
    "normalized normal for the given vert index"
  );
  f_tol.tolerance = FLT_EPSILON;
  calculate_vertex_normal(
    0,
    36,
    cube_indices,
    cube_vertex_positions,
    &actual_v3
  );
  vec3_create(
    0.577350269f,
    0.577350269f,
    -0.577350269f,
    &expected_v3
  );
  assert(vec3_equals_vec3(
    &actual_v3,
    &expected_v3,
    &f_tol
  ));
  PASSED

  // TODO: add more tests for normal calc
  // TEST("calculate_vertex_normal run #2");
  // PASSED

  /*

    PRECISION

  */

  TEST(
    "diff_is_within_tolerance should return true when comparing\n"
    "1.5 with 2 with a tolerance of 0.6"
  );
  assert(diff_is_within_tolerance(
    1.5f,
    2.0f,
    0.6f
  ));
  PASSED
  
  TEST(
    "diff_is_within_tolerance should return true when comparing\n"
    "10000 and 10000.001 and a tolerance of 0.01"
  );
  assert(diff_is_within_tolerance(
    10000,
    10000.001f,
    0.01f
  ));
  PASSED

  TEST(
    "diff_is_within_tolerance should return true when comparing\n"
    "0.00010000f and 0.00010001f and a tolerance of FLT_EPSILON"
  );
  assert(diff_is_within_tolerance(
    0.00010000f,
    0.00010001f,
    FLT_EPSILON
  ));
  PASSED

  /*

    VECTORS

  */

  TEST("vec3_dot should calculate the dot product of 2 vectors");
  vec3_create(1.0f, 1.0f, 2.0f, &actual_v3b);
  vec3_create(4.2f, 35.3f, 0.1f, &actual_v3);
  assert(diff_is_within_tolerance(
    vec3_dot(&actual_v3, &actual_v3b),
    39.7f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec3_cross should calculate the cross product");
  f_tol.tolerance = FLT_EPSILON;
  vec3_create(5, 2.1f, 2.2f, &actual_v3);
  vec3_create(-2, -8, 3.8f, &actual_v3b);
  vec3_cross(&actual_v3, &actual_v3b, &actual_v3);
  vec3_create(25.58, -23.4, -35.8, &expected_v3);
  assert(vec3_equals_vec3(&actual_v3, &expected_v3, &f_tol));
  PASSED

  TEST("vec3_minus_vec3 should subtract the 2nd vec3 from the 1st");
  f_tol.tolerance = FLT_EPSILON;
  vec3_create(3.4f, 1, -1.002f, &actual_v3);
  vec3_create(1, 0, -1, &actual_v3b);
  vec3_minus_vec3(&actual_v3, &actual_v3b, &actual_v3);
  vec3_create(2.4f, 1, -0.002f, &expected_v3);
  assert(vec3_equals_vec3(&actual_v3, &expected_v3, &f_tol));
  PASSED

  TEST("vec3_plus_vec3 should add the vectors");
  f_tol.tolerance = FLT_EPSILON * 10;
  vec3_create(3.4f, 1, -1.002f, &actual_v3);
  vec3_create(1, 0, -1, &actual_v3b);
  vec3_plus_vec3(&actual_v3, &actual_v3b, &actual_v3);
  vec3_create(4.4f, 1, -2.002f, &expected_v3);
  assert(vec3_equals_vec3(&actual_v3, &expected_v3, &f_tol));
  PASSED

  TEST("vec3_mean should calculate the mean vec3");
  f_tol.tolerance = FLT_EPSILON;
  vec3 vecs[4] = {
    { 2.3f, 4, 9 },
    { -2, 2.1f, 2.2f },
    { 0.1f, 0.1f, 0.1f },
    { 2, 0.8f, 3 }
  };
  vec3_mean(vecs, 4, &actual_v3);
  vec3_create(0.6f, 1.75f, 3.575f, &expected_v3);
  assert(vec3_equals_vec3(&actual_v3, &expected_v3, &f_tol));
  PASSED

  TEST("vec3_magnitude should calculate the mag of the vec3");
  f_tol.tolerance = FLT_EPSILON;
  vec3_create(4.5f, 8, -2.2f, &actual_v3);
  float actual_magnitude = vec3_magnitude(&actual_v3);
  assert(diff_is_within_tolerance(
    9.438749917229f,
    actual_magnitude,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec3_normalize should normalize the vec3");
  f_tol.tolerance = FLT_EPSILON;
  vec3_create(23.2f, 12.49f, 92.3f, &actual_v3);
  vec3_create(
    0.2416991f,
    0.1301216f,
    0.9615872f,
    &expected_v3
  );
  vec3_normalize(&actual_v3, &actual_v3);
  assert(vec3_equals_vec3(
    &expected_v3,
    &actual_v3,
    &f_tol
  ));
  PASSED

  /*

    MATRICES (remember, column-first buffers!)

  */

  TEST("m2x2_deterimant should correctly calculate the determinant");
  m2x2_create(
    3, 2,
    2, -2,
    &actual_m2
  );
  float actual = m2x2_determinant(&actual_m2);
  float expected = -10;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3_determinant should calculate the determinant");
  f_tol.tolerance = FLT_EPSILON * 10;
  m3x3_create(
    3, 1.3f, 4,
    0, 1.2f, 3,
    -1, 0.3f, 0.3f,
    &actual_m3
  );
  assert(diff_is_within_tolerance(
    -0.72f,
    m3x3_determinant(&actual_m3),
    f_tol.tolerance
  ));
  PASSED

  TEST("m3x3_minor should calculate the minor for the given r and c");
  m3x3_create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  float actual = m3x3_minor(2, 1, &actual_m3);
  float expected = -10;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3_minor run #2");
  m3x3_create(
    2, 3, 3.4,
    2, -2, 1,
    1, 1, 2,
    &actual_m3
  );
  float actual = m3x3_minor(1, 2, &actual_m3);
  float expected = -1;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3_minor run #3");
  m3x3_create(
    2, 3, 3.4,
    2, -2, 1,
    1, 1, 2,
    &actual_m3
  );
  float actual = m3x3_minor(0, 0, &actual_m3);
  float expected = -5;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3_minors should create a 3x3 of minors for the original");
  f_tol.tolerance = FLT_EPSILON;
  m3x3_create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  m3x3_create(
    2, 2, 2,
    -2, 3, 3,
    0, -10, 0,
    &expected_m3
  );
  m3x3_minors(
    &actual_m3,
    &actual_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST("m3x3_cofactors should create a cofactor matrix of the original");
  f_tol.tolerance = FLT_EPSILON;
  m3x3_create(
    2, 2, 2,
    -2, 3, 3,
    0, -10, 0,
    &actual_m3
  );
  m3x3_create(
    2, -2, 2,
    2, 3, -3,
    0, 10, 0,
    &expected_m3
  );
  m3x3_cofactors(
    &actual_m3,
    &actual_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST("m3x3_transpose should transpose the matrix");
  f_tol.tolerance = FLT_EPSILON;
  m3x3_create(
    2, 2, 0,
    -2, 3, 10,
    2, -3, 0,
    &actual_m3
  );
  m3x3_transpose(
    &actual_m3,
    &actual_m3
  );
  m3x3_create(
    2, -2, 2,
    2, 3, -3,
    0, 10, 0,
    &expected_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST("m3x3_inverse should correctly invert the matrix");
  f_tol.tolerance = FLT_EPSILON * 100;
  m3x3_create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  m3x3_inverse(
    &actual_m3,
    &actual_m3
  );
  m3x3_create(
    0.2f, 0.2f, 0,
    -0.2f, 0.3f, 1,
    0.2f, -0.3f, 0,
    &expected_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST(
    "m4x4_x_point should correctly multiply a 4d matrix by a\n"
    "vec3 with an assumed w comp of 1"
  );
  f_tol.tolerance = FLT_EPSILON * 100;
  vec3_create(6.0f, -20.0f, -14.33f, &actual_v3);
  m4x4_create(
    1, 0, 0, 4.0f,
    0, 1, -2.2f, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &actual_m4
  );
  m4x4_x_point(&actual_m4, &actual_v3, &actual_v3);
  vec3_create(10, 11.526f, -14.33f, &expected_v3);
  assert(vec3_equals_vec3(&expected_v3, &actual_v3, &f_tol));
  PASSED

  TEST("m4x4_create should fill the matrix buffer column-first");
  f_tol.tolerance = FLT_EPSILON;
  m4x4_create(
    1, 1, 0, 0,
    6, 6, 0, 0,
    7, 7, 1, 1,
    3, 3, 4, 5,
    &actual_m4
  );
  float expected_m4_data[16] = {
    1, 6, 7, 3,
    1, 6, 7, 3,
    0, 0, 1, 4,
    0, 0, 1, 5
  };
  memcpy(&expected_m4.data, &expected_m4_data[0], sizeof(expected_m4_data));
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED
  
  TEST("m4x4_identity should create a 4d identity matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4_identity(&actual_m4);
  m4x4_create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4_translation should create a translation matrix");
  f_tol.tolerance = FLT_EPSILON;
  vec3 t = { 2.355f, 30, 1 };
  m4x4_translation(&t, &actual_m4);
  m4x4_create(
    1, 0, 0, 2.355f,
    0, 1, 0, 30,
    0, 0, 1, 1,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4_transpose should transpose the matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4_create(
    1, 0, 0, 0,
    2, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 1,
    &actual_m4
  );
  m4x4_transpose(&actual_m4);
  m4x4_create(
    1, 2, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4_scaling should create a scaling matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4_scaling(3, &actual_m4);
  m4x4_create(
    3, 0, 0, 0,
    0, 3, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  // TEST("m4x4_rotation should create a working rotation matrix");
  // // NOTE: math.h trig fns not very accurate (eg cos(90) != 0)
  // // but idc really so set tolerance higher
  // f_tol.tolerance = FLT_EPSILON * 1000;
  // vec4 axis;
  // m4x4 rotation;
  // vec4_point(1.0f, 0.0f, 0.0f, &actual_v3);
  // vec4_vector(0.0f, 1.0f, 0.0f, &axis);
  // m4x4_rotation(deg_to_rad(90), &axis, &rotation);
  // m4x4_x_vec4(&rotation, &actual_v3, &actual_v3);
  // vec4_point(0.0f, 0.0f, -1.0f, &expected_v3);
  // assert(vec4_equals_vec4(&expected_v3, &actual_v3, &f_tol));
  // PASSED

  // TEST("m4x4_rotation run #2");
  // f_tol.tolerance = FLT_EPSILON * 1000;
  // vec4 axis;
  // m4x4 rotation;
  // vec4_vector(1.0f, 1.0f, 0, &axis);
  // vec4_normalize(&axis);
  // vec4_point(1.0f, 0.0f, 0.0f, &actual_v3);
  // m4x4_rotation(deg_to_rad(180), &axis, &rotation);
  // m4x4_x_vec4(&rotation, &actual_v3, &actual_v3);
  // vec4_point(0.0f, 1.0f, 0, &expected_v3);
  // assert(vec4_equals_vec4(&expected_v3, &actual_v3, &f_tol));
  // PASSED

  TEST("m4x4_view should construct an inverted space transform matrix");
  f_tol.tolerance = FLT_EPSILON;
  vec3 up, right, forward;
  vec3_create(0.0f, 1.0f, 0.0f, &up);
  vec3_create(1.0f, 0.0f, 0.0f, &right);
  vec3_create(0.0f, 0.0f, -1.0f, &forward);
  m4x4_view(
    &right,
    &up,
    &forward,
    &actual_m4
  );
  m4x4_create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, -1, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    &actual_m4,
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
    &actual_m4
  );
  m4x4_create(
    1, 0, 0, 0.3f,
    0, 1, 0, 0,
    0, 0, 1, -1.3f,
    0, 0, 0, 1,
    &actual_m4b
  );
  m4x4_x_m4x4(
    &actual_m4,
    &actual_m4b,
    &actual_m4
  );
  m4x4_create(
    1, 0.3f, 3, -3.6f,
    0, -0.2f, 1, -1.3f,
    -2.2f, 0, 1, -1.96f,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &actual_m4,
    &expected_m4,
    &f_tol
  ));
  PASSED

  /*

    VIEWPORT

  */

  TEST("setting viewport width should set perspective_needs_recalculating to true");
  viewport vwprt = {0};
  viewport__set_width(100, &vwprt);
  assert(viewport__perspective_needs_recalculating(&vwprt));
  PASSED

  TEST("setting viewport height should set perspective_needs_recalculating to true");
  viewport vwprt = {0};
  viewport__set_height(200, &vwprt);
  assert(viewport__perspective_needs_recalculating(&vwprt));
  PASSED

  TEST("viewport__get_aspect_ratio should return the viewport width / height");
  viewport vwprt = {0};
  viewport__set_width(1920, &vwprt);
  viewport__set_height(1080, &vwprt);
  assert(diff_is_within_tolerance(
    1.777777777777f,
    viewport__get_aspect_ratio(&vwprt),
    FLT_EPSILON
  ));
  PASSED

  /*

    CAMERA

  */

  TEST("camera__calculate_lookat should create a lookat matrix");
  f_tol.tolerance = FLT_EPSILON;
  vec3 target = { 0.0f, 0.5f, -1.0f };
  vec3 world_up = { 0.0f, 1.0f, 0.0f };
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
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    camera__get_lookat(&cam),
    &f_tol
  ));
  PASSED

  TEST("moving the camera should set view_needs_recalculating to true");
  camera cam;
  camera__init(&cam);
  camera__set_position(0.0f, 1.0f, 1.2f, &cam);
  assert(camera__lookat_needs_recalculating(&cam));
  PASSED

  TEST("changing the camera look target should set view_needs_recalculating to true");
  camera cam;
  camera__init(&cam);
  vec3 target = { 0.3f, 0.0f, 3.0f };
  camera__set_look_target(&target, &cam);
  assert(camera__lookat_needs_recalculating(&cam));
  PASSED

  TEST("calculating the camera view should set view_needs_recalculating to false");
  camera cam;
  camera__init(&cam);
  vec3 target = { 0.0f, 0.5f, -1.0f };
  vec3 world_up = { 0.0f, 1.0f, 0.0f };
  camera__set_position(0.0f, 0.25f, 0.5f, &cam);
  camera__set_look_target(&target, &cam);
  camera__calculate_lookat(&world_up, &cam);
  assert(!camera__lookat_needs_recalculating(&cam));
  PASSED

  TEST("setting horiz fov should set perspective_needs_recalculating to true");
  camera cam;
  camera__init(&cam);
  camera__set_horizontal_fov_in_deg(45, &cam);
  assert(camera__perspective_needs_recalculating(&cam));
  PASSED

  TEST(
    "camera__calculate_perspective should create a working perspective\n"
    "projection matrix"
  );
  f_tol.tolerance = FLT_EPSILON * 1000;
  camera cam;
  camera__init(&cam);
  camera__set_horizontal_fov_in_deg(75, &cam);
  camera__set_near_clip_distance(0.1f, &cam);
  camera__set_far_clip_distance(10.0f, &cam);
  viewport vwprt;
  viewport__set_width(1920, &vwprt);
  viewport__set_height(1080, &vwprt);
  camera__calculate_perspective(&vwprt, &cam);
  m4x4_create(
    1.303221243f, 0, 0, 0,
    0, 2.31683776546f, 0, 0,
    0, 0, -1.0202020202f, -0.20202020202f,
    0, 0, -1, 0,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    camera__get_perspective(&cam),
    &f_tol
  ));
  PASSED

  TEST(
    "camera__calculate_perspective should set perspective_needs_recalculating\n"
    "to false on the camera and viewport"
  );
  camera cam;
  camera__init(&cam);
  camera__set_horizontal_fov_in_deg(75, &cam);
  camera__set_near_clip_distance(0.1f, &cam);
  camera__set_far_clip_distance(10.0f, &cam);
  viewport vwprt;
  viewport__set_width(1920, &vwprt);
  viewport__set_height(1080, &vwprt);
  camera__calculate_perspective(&vwprt, &cam);
  assert(!camera__perspective_needs_recalculating(&cam));
  assert(!viewport__perspective_needs_recalculating(&vwprt));
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
    .position = { 0.3f, -2.0f, -2.0f },
    .rotation_in_deg = { 0, 0, 45 },
    .scale = 1.3f
  };
  space__create_model(
    &DEFAULT_WORLDSPACE,
    &t,
    &actual_m4
  );
  m4x4_create(
    0.919187997f, 0, 0.919187997f, 0.3f,
    0, 1.3f, 0, -2,
    -0.919187997f, 0, 0.919187997f, -2,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    &actual_m4,
    &f_tol
  ));
  PASSED

  printf("\n\n");
  printf("_____________________________________\n");
  printf("_________ALL_TESTS_PASSED_:)_________\n");
  printf("_____________________________________\n");
}

int m2x2_equals_m2x2(
  const m2x2 *m0,
  const m2x2 *m1,
  const float_tolerance *ft
) {
  for (int i = 0; i < 4; i++) {
    if (!ft->within_tolerance(
      m0->data[i],
      m1->data[i],
      ft->tolerance
    )) return 0;
  }
  return 1;
}

int m3x3_equals_m3x3(
  const m3x3 *m0,
  const m3x3 *m1,
  const float_tolerance *ft
) {
  for (int i = 0; i < 9; i++) {
    if (!ft->within_tolerance(
      m0->data[i],
      m1->data[i],
      ft->tolerance
    )) return 0;
  }
  return 1;
}

int m4x4_equals_m4x4(
  const m4x4 *m0,
  const m4x4 *m1,
  const float_tolerance *ft
) {
  for (int i = 0; i < 16; i++) {
    if (!ft->within_tolerance(
      m0->data[i],
      m1->data[i],
      ft->tolerance
    )) return 0;
  }
  return 1;
}

void print_m3x3(const char *name, const m3x3 *m) {
  printf(name);
  printf("%.12f, ", m->data[0]);
  printf("%.12f, ", m->data[3]);
  printf("%.12f\n", m->data[6]);
  printf("%.12f, ", m->data[1]);
  printf("%.12f, ", m->data[4]);
  printf("%.12f\n", m->data[7]);
  printf("%.12f, ", m->data[2]);
  printf("%.12f, ", m->data[5]);
  printf("%.12f\n", m->data[8]);
}

// TODO: this prints row-first! we're column-first! transpose in ur head
void print_m4x4(const char *name, const m4x4 *m) {
  printf(name);
  for (int i = 0; i < 16; i++) {
    printf("%.12f, ", m->data[i]);
    if (
      i == 3 ||
      i == 7 ||
      i == 11
    ) printf("\n");
  }
}

void print_vec3(const char *name, const vec3 *t) {
  printf(name);
  for (int i = 0; i < 4; i++) {
    printf("%.12f, ", (&t->x)[i]);
  }
}