#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "tail.h"

// TODO: could clean & reduce loc by using compound literals to
// pass anon struct for a lot of these. project for a lazy day

#define TEST(msg) { printf(msg)
#define PASSED printf(": PASSED\n"); }

int m2x2_equals_m2x2(
  const M2x2 *m0,
  const M2x2 *m1,
  const FloatTolerance *ft
);
int m3x3_equals_m3x3(
  const M3x3 *m0,
  const M3x3 *m1,
  const FloatTolerance *ft
);
int m4x4_equals_m4x4(
  const M4x4 *m0,
  const M4x4 *m1,
  const FloatTolerance *ft
);
void print_m3x3(const char *name, const M3x3 *m);
void print_m4x4(const char *name, const M4x4 *m);
void print_vec3(const char *name, Vec3 t);

M2x2 expected_m2, actual_m2;
M3x3 expected_m3, actual_m3;
M4x4 expected_m4, actual_m4, actual_m4b;

FloatTolerance f_tol = {
  .within_tolerance = diff_is_within_tolerance,
  .tolerance = FLT_EPSILON
};

const CoordinateSpace DEFAULT_WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, 1 }
};

int main(void) {

  /*
  
    GAMEPAD
  
  */

  TEST("button_is_down should return 1 when the selected button is down");
  Gamepad gamepad = {0};
  gamepad.buttons |= 1 << BUTTON_Y;
  assert(button_is_down(BUTTON_Y, &gamepad) == 1);
  PASSED

  TEST("button_is_up should return 0 when the selected button is up");
  Gamepad gamepad = {0};
  assert(button_is_up(BUTTON_Y, &gamepad) == 1);
  PASSED

  TEST("button_was_released should return 1 when the selected button is up but was down");
  Gamepad gamepad = {0};
  gamepad.previous_buttons |= 1 << BUTTON_Y;
  assert(button_was_released(BUTTON_Y, &gamepad) == 1);
  PASSED

  /*

    MATH

  */

  TEST("iclamp should return 3 when clamping 5 between 0 and 3");
  assert(iclamp(5, 0, 3) == 3);
  PASSED

  TEST("iclamp should return 1 when clamping 2 between 0 and 1");
  assert(iclamp(2, 0, 1) == 1);
  PASSED
  
  TEST("fclamp should return 1 when clamping 1.43 between 0 and 1");
  assert(diff_is_within_tolerance(
    fclamp(1.43f, 0, 1),
    1,
    FLT_EPSILON
  ));
  PASSED

  TEST("fclamp should return 0.77 when clamping 0.77 between 0.5f and 29");
  assert(diff_is_within_tolerance(
    fclamp(0.77f, 0.5f, 29),
    0.77f,
    FLT_EPSILON
  ));
  PASSED

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

  TEST("vec3__dot should calculate the dot product of 2 vectors");
  Vec3 a = {1.0f, 1.0f, 2.0f};
  Vec3 b = {4.2f, 35.3f, 0.1f};
  assert(diff_is_within_tolerance(
    vec3__dot(a, b),
    39.7f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec2__dot should calcualte the dot btw 2 vec2");
  Vec2 a = { 1, 0 };
  Vec2 b = { 1, 0 };
  float dot = vec2__dot(a, b);
  assert(diff_is_within_tolerance(
    dot,
    1.0f,
    FLT_EPSILON
  ));
  PASSED

  const float NORM_1_1_V2 = 0.7071067811865475f;

  TEST("vec2__dot run #2");
  Vec2 a = { 1, 0 };
  Vec2 b = { NORM_1_1_V2, NORM_1_1_V2 };
  float dot = vec2__dot(a, b);
  assert(diff_is_within_tolerance(
    dot,
    NORM_1_1_V2,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec2__dot run #3");
  Vec2 a = { NORM_1_1_V2, NORM_1_1_V2 };
  Vec2 b = { -NORM_1_1_V2, -NORM_1_1_V2 };
  float dot = vec2__dot(a, b);
  assert(diff_is_within_tolerance(
    dot,
    -1.0f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec2__fewest_rads_btw_vec2s should calculate the angle btw 2 vec2s");
  Vec2 a = { 1, 0 };
  Vec2 b = { 0, 1 };
  assert(diff_is_within_tolerance(
    rad_to_deg(vec2__fewest_rads_btw_vec2s(a, b)),
    90.0f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec2__fewest_rads_btw_vec2s run #2");
  Vec2 a = { 0, 1 };
  Vec2 b = { NORM_1_1_V2, -NORM_1_1_V2 };
  assert(diff_is_within_tolerance(
    rad_to_deg(vec2__fewest_rads_btw_vec2s(a, b)),
    135.0f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec3__cross should calculate the cross product");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 a = { 5, 2.1f, 2.2f };
  Vec3 b = { -2, -8, 3.8f };
  Vec3 actual = vec3__cross(a, b);
  Vec3 expected = { 25.58f, -23.4f, -35.8f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("vec3_minus_vec3 should subtract the 2nd vec3 from the 1st");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 a = { 3.4f, 1, -1.002f };
  Vec3 b = { 1, 0, -1 };
  Vec3 actual = vec3_minus_vec3(a, b);
  Vec3 expected = { 2.4f, 1, -0.002f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("vec3_plus_vec3 should add the vectors");
  f_tol.tolerance = FLT_EPSILON * 10; // TODO: this is necessary?
  Vec3 a = { 3.4f, 1, -1.002f };
  Vec3 b = { 1, 0, -1 };
  Vec3 actual = vec3_plus_vec3(a, b);
  Vec3 expected = { 4.4f, 1, -2.002f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("vec3__mean should calculate the mean vec3");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 vecs[4] = {
    { 2.3f, 4, 9 },
    { -2, 2.1f, 2.2f },
    { 0.1f, 0.1f, 0.1f },
    { 2, 0.8f, 3 }
  };
  Vec3 actual = vec3__mean(vecs, 4);
  Vec3 expected = { 0.6f, 1.75f, 3.575f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("vec3__magnitude should calculate the mag of the vec3");
  f_tol.tolerance = FLT_EPSILON;
  float actual = vec3__magnitude((Vec3){ 4.5f, 8, -2.2f });
  assert(diff_is_within_tolerance(
    actual,
    9.438749917229f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec3__normalize should normalize the vec3");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 a = { 23.2f, 12.49f, 92.3f };
  Vec3 actual = vec3__normalize(a);
  Vec3 expected = {
    0.2416991f,
    0.1301216f,
    0.9615872f
  };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("vec2__magnitude should calculate the mag of the vec2");
  f_tol.tolerance = FLT_EPSILON;
  assert(diff_is_within_tolerance(
    vec2__magnitude((Vec2){ 0.23f, 1.00f }),
    1.02610915599f,
    FLT_EPSILON
  ));
  PASSED

  TEST("vec2__normalize should normalize the vec2");
  f_tol.tolerance = FLT_EPSILON * 100;
  Vec2 actual = vec2__normalize((Vec2){ 0.23f, 1.00f });
  assert(vec_equals_vec(
    &actual.x,
    &((Vec2){ 0.224147f, 0.974555f }).x,
    2,
    &f_tol
  ));
  PASSED

  TEST("vec2__dist_from_point_to_line should correctly calclate the distance");
  Vec2 test_position = { 4.0f, 2.3f };
  Vec2 p0 = { -2, 2 };
  Vec2 p1 = { 0, 0 };
  float expected = 4.45477f;
  float actual = vec2__dist_from_point_to_line(
    p0,
    p1,
    test_position
  );
  printf("\n\nFLT_EPSILON %.8f", FLT_EPSILON * 100);
  printf("\nexpected %.8f", expected);
  printf("\nactual %.8f\n\n", actual);
  assert(diff_is_within_tolerance(
    actual,
    expected,
    FLT_EPSILON * 100
  ));
  PASSED

  /*

    MATRICES (remember, column-first buffers!)

  */

  TEST("m2x2_deterimant should correctly calculate the determinant");
  m2x2__create(
    3, 2,
    2, -2,
    &actual_m2
  );
  float actual = m2x2__determinant(&actual_m2);
  float expected = -10;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3__determinant should calculate the determinant");
  f_tol.tolerance = FLT_EPSILON * 10;
  m3x3__create(
    3, 1.3f, 4,
    0, 1.2f, 3,
    -1, 0.3f, 0.3f,
    &actual_m3
  );
  assert(diff_is_within_tolerance(
    -0.72f,
    m3x3__determinant(&actual_m3),
    f_tol.tolerance
  ));
  PASSED

  TEST("m3x3__minor should calculate the minor for the given r and c");
  m3x3__create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  float actual = m3x3__minor(2, 1, &actual_m3);
  float expected = -10;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3__minor run #2");
  m3x3__create(
    2, 3, 3.4,
    2, -2, 1,
    1, 1, 2,
    &actual_m3
  );
  float actual = m3x3__minor(1, 2, &actual_m3);
  float expected = -1;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3__minor run #3");
  m3x3__create(
    2, 3, 3.4,
    2, -2, 1,
    1, 1, 2,
    &actual_m3
  );
  float actual = m3x3__minor(0, 0, &actual_m3);
  float expected = -5;
  assert(diff_is_within_tolerance(actual, expected, FLT_EPSILON));
  PASSED

  TEST("m3x3__minors should create a 3x3 of minors for the original");
  f_tol.tolerance = FLT_EPSILON;
  m3x3__create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  m3x3__create(
    2, 2, 2,
    -2, 3, 3,
    0, -10, 0,
    &expected_m3
  );
  m3x3__minors(
    &actual_m3,
    &actual_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST("m3x3__cofactors should create a cofactor matrix of the original");
  f_tol.tolerance = FLT_EPSILON;
  m3x3__create(
    2, 2, 2,
    -2, 3, 3,
    0, -10, 0,
    &actual_m3
  );
  m3x3__create(
    2, -2, 2,
    2, 3, -3,
    0, 10, 0,
    &expected_m3
  );
  m3x3__cofactors(
    &actual_m3,
    &actual_m3
  );
  assert(m3x3_equals_m3x3(
    &expected_m3,
    &actual_m3,
    &f_tol
  ));
  PASSED

  TEST("m3x3__transpose should transpose the matrix");
  f_tol.tolerance = FLT_EPSILON;
  m3x3__create(
    2, 2, 0,
    -2, 3, 10,
    2, -3, 0,
    &actual_m3
  );
  m3x3__transpose(
    &actual_m3,
    &actual_m3
  );
  m3x3__create(
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

  TEST("m3x3__inverse should correctly invert the matrix");
  f_tol.tolerance = FLT_EPSILON * 100;
  m3x3__create(
    3, 0, 2,
    2, 0, -2,
    0, 1, 1,
    &actual_m3
  );
  m3x3__inverse(
    &actual_m3,
    &actual_m3
  );
  m3x3__create(
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
  m4x4__create(
    1, 0, 0, 4.0f,
    0, 1, -2.2f, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &actual_m4
  );
  Vec3 actual = m4x4_x_point(
    &actual_m4,
    (Vec3){ 6.0f, -20.0f, -14.33f }
  );
  Vec3 expected = { 10, 11.526f, -14.33f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("m4x4__create should fill the matrix buffer column-first");
  f_tol.tolerance = FLT_EPSILON;
  m4x4__create(
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
  
  TEST("m4x4__identity should create a 4d identity matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4__identity(&actual_m4);
  m4x4__create(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4__translation should create a translation matrix");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 t = { 2.355f, 30, 1 };
  m4x4__translation(t, &actual_m4);
  m4x4__create(
    1, 0, 0, 2.355f,
    0, 1, 0, 30,
    0, 0, 1, 1,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4__transpose should transpose the matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4__create(
    1, 0, 0, 0,
    2, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 1,
    &actual_m4
  );
  m4x4__transpose(&actual_m4);
  m4x4__create(
    1, 2, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4__scaling should create a scaling matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4__scaling(3, &actual_m4);
  m4x4__create(
    3, 0, 0, 0,
    0, 3, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(&expected_m4, &actual_m4, &f_tol));
  PASSED

  TEST("m4x4__rotation should create a working rotation matrix");
  // NOTE: math.h trig fns not very accurate (eg cos(90) != 0)
  // but idc really so set tolerance higher
  f_tol.tolerance = FLT_EPSILON * 1000;
  M4x4 rotation;
  Vec3 a = { 1.0f, 0.0f, 0.0f };
  Vec3 axis = { 0.0f, 1.0f, 0.0f };
  m4x4__rotation(deg_to_rad(90), axis, &rotation);
  Vec3 actual = m4x4_x_point(&rotation, a);
  Vec3 expected = { 0.0f, 0.0f, -1.0f };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("m4x4__rotation run #2");
  f_tol.tolerance = FLT_EPSILON * 1000;
  M4x4 rotation;
  m4x4__rotation(
    deg_to_rad(180),
    vec3__normalize((Vec3){ 1, 1, 0 }),
    &rotation
  );
  Vec3 actual = m4x4_x_point(
    &rotation,
    (Vec3){ 1, 0, 0 }
  );
  Vec3 expected = { 0, 1, 0 };
  assert(vec_equals_vec(&actual.x, &expected.x, 3, &f_tol));
  PASSED

  TEST("m4x4__view should conan inverted space transform matrix");
  f_tol.tolerance = FLT_EPSILON;
  m4x4__view(
    (Vec3){ 1, 0, 0 },
    (Vec3){ 0, 1, 0 },
    (Vec3){ 0, 0, -1 },
    &actual_m4
  );
  m4x4__create(
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
  m4x4__create(
    1, 0.3f, 3, 0,
    0, -0.2f, 1, 0,
    -2.2f, 0, 1, 0,
    0, 0, 0, 1,
    &actual_m4
  );
  m4x4__create(
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
  m4x4__create(
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

  TEST(
    "m4x4__sub3x3_from00 should extract the 0,0 index 3x3 submatrix"
    "from the source 4x4"
  );
  f_tol.tolerance = FLT_EPSILON;
  M4x4 m;
  m4x4__create(
    1, 0.3f, 3, 0,
    0, -0.2f, 1, 0,
    -2.2f, 0, 1, 0,
    0, 0, 0, 1,
    &m
  );
  M3x3 actual_m3;
  m4x4__sub3x3_from00(&m, &actual_m3);
  M3x3 expected_m3;
  m3x3__create(
    1, 0.3f, 3,
    0, -0.2f, 1,
    -2.2f, 0, 1,
    &expected_m3
  );
  assert(m3x3_equals_m3x3(
    &actual_m3,
    &expected_m3,
    &f_tol
  ));
  PASSED

  /*

    VIEWPORT

  */

  TEST("setting viewport width should set perspective_needs_recalculating to true");
  Viewport vwprt = {0};
  viewport__set_width(100, &vwprt);
  assert(viewport__perspective_needs_recalculating(&vwprt));
  PASSED

  TEST("setting viewport height should set perspective_needs_recalculating to true");
  Viewport vwprt = {0};
  viewport__set_height(200, &vwprt);
  assert(viewport__perspective_needs_recalculating(&vwprt));
  PASSED

  TEST("viewport__get_aspect_ratio should return the viewport width / height");
  Viewport vwprt = {0};
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
  Camera cam;
  cam.position = (Vec3){ 0.0f, 0.25f, 0.5f };
  cam.look_target = (Vec3){ 0.0f, 0.5f, -1.0f };
  camera__calculate_lookat((Vec3){ 0.0f, 1.0f, 0.0f }, &cam);
  m4x4__create(
    1, 0, 0, 0,
    0, 0.98639392853f, 0.16439899802f, -0.3287979960f,
    0, -0.16439899802f, 0.98639392853f, -0.4520971775f,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    &cam.lookat,
    &f_tol
  ));
  PASSED

  TEST(
    "camera__calculate_perspective should create a working perspective\n"
    "projection matrix"
  );
  f_tol.tolerance = FLT_EPSILON * 1000;
  Camera cam;
  cam.horizontal_fov_in_deg = 75;
  cam.near_clip_distance = 0.1f;
  cam.far_clip_distance = 10;
  Viewport vwprt;
  viewport__set_width(1920, &vwprt);
  viewport__set_height(1080, &vwprt);
  camera__calculate_perspective(&vwprt, &cam);
  m4x4__create(
    1.303221243f, 0, 0, 0,
    0, 2.31683776546f, 0, 0,
    0, 0, -1.0202020202f, -0.20202020202f,
    0, 0, -1, 0,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    &cam.projection,
    &f_tol
  ));
  PASSED

  /*

    SPACE

  */

  TEST(
    "space__ccw_angle_rotate should rotate the provided point n rads around the\n"
    "provided rotation axis"
  ); 
  f_tol.tolerance = FLT_EPSILON;
  Vec3 t = { 1, 0, 0 };
  Vec3 expected = { 0, 0, -1 };
  Vec3 rotated = space__ccw_angle_rotate(
    (Vec3){ 0, 1, 0 },
    M_PI * 0.5f,
    t
  );
  assert(vec_equals_vec(
    &rotated.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("space__ccw_quat_rotate run #2");
  f_tol.tolerance = FLT_EPSILON * 10;
  Vec3 t = { 1, 0, 0 };
  Quaternion q0, q1, q2;
  q0 = quaternion__create(
    (Vec3){ 0, 1, 0 },
    M_PI * 0.25f
  );
  q1 = quaternion__create(
    (Vec3){ 0, 1, 0 },
    M_PI * 0.25f
  );
  q2 = quaternion__multiply(q0, q1);
  Vec3 rotated = space__ccw_quat_rotate(q2, t);
  Vec3 expected = { 0, 0, -1 };
  assert(vec_equals_vec(
    &rotated.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST(
    "space__create_model should use a transform to create a compound\n"
    "matrix for rotation around the z axis, scale, and translation"
  );
  f_tol.tolerance = FLT_EPSILON * 10;
  Transform t = {
    .position = { 0.3f, -2.0f, -2.0f },
    .rotation = quaternion__create(
      (Vec3){ 0, 0, 1 },
      deg_to_rad(45)
    ),
    .scale = 1.3f
  };
  space__create_model(
    &DEFAULT_WORLDSPACE,
    &t,
    &actual_m4
  );
  m4x4__create(
    0.9192387f, -0.9192387f, 0, 0.3f,
    0.9192387f, 0.9192387f, 0, -2,
    0, 0, 1.3f, -2,
    0, 0, 0, 1,
    &expected_m4
  );
  assert(m4x4_equals_m4x4(
    &expected_m4,
    &actual_m4,
    &f_tol
  ));
  PASSED

  // TODO: finish this test smh
  // TEST("space__create_model run #2");
  // f_tol.tolerance = FLT_EPSILON * 1000;
  // Transform t = {
  //   .position = { 0, 0, 1 },
  //   .rotation_in_deg = { 90, 0, 0 },
  //   .scale = 1
  // };
  // space__create_model(
  //   &DEFAULT_WORLDSPACE,
  //   &t,
  //   &actual_m4
  // );
  // m4x4__create(
  //   1, 0, 0, 0,
  //   0, 1, 0, 0,
  //   0, 0, 1, 1,
  //   0, 0, 0, 1,
  //   &expected_m4
  // );
  // assert(m4x4_equals_m4x4(
  //   &expected_m4,
  //   &actual_m4,
  //   &f_tol
  // ));
  // PASSED

  printf("\n\n");
  printf("_____________________________________\n");
  printf("_________ALL_TESTS_PASSED_:)_________\n");
  printf("_____________________________________\n");

  return 0;
}

int m2x2_equals_m2x2(
  const M2x2 *m0,
  const M2x2 *m1,
  const FloatTolerance *ft
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
  const M3x3 *m0,
  const M3x3 *m1,
  const FloatTolerance *ft
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
  const M4x4 *m0,
  const M4x4 *m1,
  const FloatTolerance *ft
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

void print_m3x3(const char *name, const M3x3 *m) {
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

void print_m4x4(const char *name, const M4x4 *m) {
  M4x4 transposed = {0};
  memcpy(&transposed.data, &m->data, sizeof(M4x4));
  m4x4__transpose(&transposed);
  printf(name);
  printf("\n");
  for (int i = 0; i < 16; i++) {
    printf("%.12f, ", transposed.data[i]);
    if (
      i == 3 ||
      i == 7 ||
      i == 11
    ) printf("\n");
  }
  printf("\n\n");
}

void print_vec3(const char *name, Vec3 t) {
  printf(name);
  printf("{ %.12f, %.12f, %.12f }", t.x, t.y, t.z);
}