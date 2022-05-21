#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "tail.h"
#include "test_data.h"
#include "normals.h"
#include "parser.h"

#define TEST(msg) { printf(msg)
#define PASSED printf(": PASSED\n"); }
#define QUIT return 0;

// int diff_is_within_tolerance(float a, float b, float tol) {
//   float abs_diff = fabs(a - b);
//   return abs_diff <= tol ? 1 : 0;
// }

// int vec3_equals_vec3(const vec3 *t0, const vec3 *t1, float tol) {
//   if (!diff_is_within_tolerance(t0->x, t1->x, tol)) return 0;
//   if (!diff_is_within_tolerance(t0->y, t1->y, tol)) return 0;
//   if (!diff_is_within_tolerance(t0->z, t1->z, tol)) return 0;
//   return 1;
// }

struct float_tolerance f_tol = {
  .within_tolerance = diff_is_within_tolerance,
  .tolerance = FLT_EPSILON
};

int main() {

  TEST("tolerance check 1");
  assert(diff_is_within_tolerance(
    1.033929f,
    1.033928f,
    0.000002f
  ));
  PASSED

  TEST("tolerance check 2");
  assert(diff_is_within_tolerance(
    1.033929f,
    1.033928f,
    0.0000005f
  ) == 0);
  PASSED

  TEST("obj_vec3_line_to_vec3 should convert the str to a vec3");
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "v 0.894426 0.447216 0.000000";
  struct vec3 actual = {0};
  obj_vec3_line_to_vec3(str, &actual);
  struct vec3 expected = {
    0.894426f,
    0.447216f,
    0.000000f
  };
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  TEST("obj_vec3_line_to_vec3 run #2");
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "vn -0.5746 -0.3304 -0.7488";
  struct vec3 actual = {0};
  obj_vec3_line_to_vec3(str, &actual);
  struct vec3 expected = {
    -0.5746f,
    -0.3304f,
    -0.7488f
  };
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  TEST(
    "obj_f_line_to_3_ui_indices should convert the obj f string\n"
    "into a vec3 of indices; remember i-1"
  );
    const char *str = "f 5 2 4";
    unsigned int actual_indices[3] = {0};
    obj_f_line_to_3_ui_indices(str, actual_indices);
    unsigned int expected_indices[3] = { 4, 1, 3 };
    for (int i = 0; i < 3; i++) {
      assert(diff_is_within_tolerance(
        actual_indices[i],
        expected_indices[i],
        FLT_EPSILON
      ));
    }
  PASSED

  TEST(
    "obj_f_line_to_3_ui_indices run #2"
  );
    const char *str = "f 242 150 99";
    unsigned int actual_indices[3] = {0};
    obj_f_line_to_3_ui_indices(str, actual_indices);
    unsigned int expected_indices[3] = { 241, 149, 98 };
    for (int i = 0; i < 3; i++) {
      assert(diff_is_within_tolerance(
        actual_indices[i],
        expected_indices[i],
        FLT_EPSILON
      ));
    }
  PASSED

  TEST(
    "obj_f_n_line_to_vec3s should convert the "
    "str into two corresponding vec3s; remember i-1"
  );
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "f 10//5 11//5 9//5";
  struct vec3 actual_vi = {0};
  struct vec3 actual_vni = {0};
  obj_f_n_line_to_vec3s(
    str,
    &actual_vi,
    &actual_vni
  );
  struct vec3 expected_vi = { 9, 10, 8 };
  struct vec3 expected_vni = { 4, 4, 4 };  
  assert(vec3_equals_vec3(
    &actual_vi,
    &expected_vi,
    &f_tol
  ));
  assert(vec3_equals_vec3(
    &actual_vni,
    &expected_vni,
    &f_tol
  ));
  PASSED

  TEST(
    "obj_f_n_line_to_vec3s run #2"
  );
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "f 285//30 311//30 600//30";
  struct vec3 actual_vi = {0};
  struct vec3 actual_vni = {0};
  obj_f_n_line_to_vec3s(
    str,
    &actual_vi,
    &actual_vni
  );
  struct vec3 expected_vi = { 284, 310, 599 };
  struct vec3 expected_vni = { 29, 29, 29 };  
  assert(vec3_equals_vec3(
    &actual_vi,
    &expected_vi,
    &f_tol
  ));
  assert(vec3_equals_vec3(
    &actual_vni,
    &expected_vni,
    &f_tol
  ));
  PASSED

  TEST(
    "calculate_face_normal should calculate a normalized\n"
    "normal for the provided triangle of positions"
  );
  f_tol.tolerance = FLT_EPSILON;
  struct vec3 positions[3] = {
    { -1, 0, 0 },
    { 0, 0, 1 },
    { 0, 1, 0 }
  };
  struct vec3 actual = {0};
  struct vec3 expected = {0};
  calculate_face_normal(positions, &actual);
  vec3_create(
    -0.577350269f,
    0.577350269f,
    0.577350269f,
    &expected
  );
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  TEST("calculate_face_normal run #2");
  f_tol.tolerance = FLT_EPSILON;
  struct vec3 positions[3] = {
    { 0, 0, 0 },
    { 0, 0, -1 },
    { 0, 1, 0 }
  };
  struct vec3 actual = {0};
  calculate_face_normal(positions, &actual);
  struct vec3 expected = { 1, 0, 0 };
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  TEST(
    "calculate_vertex_normal should calculate a\n"
    "normalized normal for the given vert index"
  );
  f_tol.tolerance = FLT_EPSILON;
  struct vec3 actual = {0};
  calculate_vertex_normal(
    0,
    36,
    cube_indices,
    cube_vertex_positions,
    &actual
  );
  struct vec3 expected = {
    0.577350269f,
    0.577350269f,
    -0.577350269f,
  };
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  TEST("calculate_vertex_normal run #2");
  f_tol.tolerance = FLT_EPSILON;
  struct vec3 actual = {0};
  calculate_vertex_normal(
    4,
    18,
    triangular_pyramid_indices,
    triangular_pyramid_positions,
    &actual
  );
  struct vec3 expected = { 0, 1, 0 };
  assert(vec3_equals_vec3(
    &actual,
    &expected,
    &f_tol
  ));
  PASSED

  printf("\n\n");
  printf("_____________________________________\n");
  printf("_________ALL_TESTS_PASSED_:)_________\n");
  printf("_____________________________________\n");
}