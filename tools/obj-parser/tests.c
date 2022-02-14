#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "tail.h"
#include "types.h"
#include "parser.h"

#define TEST(msg) { printf(msg)
#define PASSED printf(": PASSED\n"); }
#define QUIT return 0;

int floats_close_enough(float a, float b, float tol) {
  float abs_diff = fabs(a - b);
  return abs_diff <= tol ? 1 : 0;
}

int vec3s_close_enough(const vec3 *t0, const vec3 *t1, float tol) {
  if (!floats_close_enough(t0->x, t1->x, tol)) return 0;
  if (!floats_close_enough(t0->y, t1->y, tol)) return 0;
  if (!floats_close_enough(t0->z, t1->z, tol)) return 0;
  return 1;
}

int main() {

  TEST("tolerance check 1");
  assert(floats_close_enough(
    1.033929f,
    1.033928f,
    0.000002f
  ));
  PASSED

  TEST("tolerance check 2");
  assert(floats_close_enough(
    1.033929f,
    1.033928f,
    0.0000005f
  ) == 0);
  PASSED

  TEST("obj_vec3_line_to_vec3 should convert the str to a vec3");
  const char *str = "v 0.894426 0.447216 0.000000";
  vec3 actual = {0};
  obj_vec3_line_to_vec3(str, &actual);
  vec3 expected = {
    0.894426f,
    0.447216f,
    0.000000f
  };
  assert(vec3s_close_enough(
    &actual,
    &expected,
    FLT_EPSILON
  ));
  PASSED

  TEST("obj_vec3_line_to_vec3 run #2");
  const char *str = "vn -0.5746 -0.3304 -0.7488";
  vec3 actual = {0};
  obj_vec3_line_to_vec3(str, &actual);
  vec3 expected = {
    -0.5746f,
    -0.3304f,
    -0.7488f
  };
  assert(vec3s_close_enough(
    &actual,
    &expected,
    FLT_EPSILON
  ));
  PASSED

  TEST(
    "obj_face_line_to_vec3s should convert the "
    "str into two corresponding vec3s; remember i-1"
  );
  const char *str = "f 10//5 11//5 9//5";
  vec3 actual_vi = {0};
  vec3 actual_vni = {0};
  obj_face_line_to_vec3s(
    str,
    &actual_vi,
    &actual_vni
  );
  vec3 expected_vi = { 9, 10, 8 };
  vec3 expected_vni = { 4, 4, 4 };  
  assert(vec3s_close_enough(
    &actual_vi,
    &expected_vi,
    FLT_EPSILON
  ));
  assert(vec3s_close_enough(
    &actual_vni,
    &expected_vni,
    FLT_EPSILON
  ));
  PASSED

  TEST(
    "obj_face_line_to_vec3s run #2"
  );
  const char *str = "f 285//30 311//30 600//30";
  vec3 actual_vi = {0};
  vec3 actual_vni = {0};
  obj_face_line_to_vec3s(
    str,
    &actual_vi,
    &actual_vni
  );
  vec3 expected_vi = { 284, 310, 599 };
  vec3 expected_vni = { 29, 29, 29 };  
  assert(vec3s_close_enough(
    &actual_vi,
    &expected_vi,
    FLT_EPSILON
  ));
  assert(vec3s_close_enough(
    &actual_vni,
    &expected_vni,
    FLT_EPSILON
  ));
  PASSED
}