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

FloatTolerance f_tol = {
  .within_tolerance = diff_is_within_tolerance,
  .tolerance = FLT_EPSILON
};

unsigned int vert_equals_vert(
  const Vertex *v0,
  const Vertex *v1,
  const FloatTolerance *f_tol
) {
  if (!vec_equals_vec(&v0->position.x, &v1->position.x, 3, f_tol)) return 0;
  if (!vec_equals_vec(&v0->normal.x, &v1->normal.x, 3, f_tol)) return 0;
  if (!vec_equals_vec(&v0->uv.x, &v1->uv.x, 2, f_tol)) return 0;
  return 1;
}

void print_vec3(const Vec3 v) {
  printf("{%.8f, %.8f, %.8f}\n", v.x, v.y, v.z);
}

void print_vert(const Vertex v) {
  printf(
    "{{%.8f,%.8f,%.8f},"
    "{%.8f,%.8f,%.8f},"
    "{%.8f,%.8f}}\n",
    v.position.x, v.position.y, v.position.z,
    v.normal.x, v.normal.y, v.normal.z,
    v.uv.x, v.uv.y
  );
}

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

  TEST("obj_float_line_to_vector should convert the str to a Vec3");
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "v 0.894426 0.447216 0.000000";
  Vec3 actual = {0};
  obj_float_line_to_vector(str, &actual.x);
  Vec3 expected = {
    0.894426f,
    0.447216f,
    0.000000f
  };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("obj_float_line_to_vector run #2");
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "vn -0.5746 -0.3304 -0.7488";
  Vec3 actual = {0};
  obj_float_line_to_vector(str, &actual.x);
  Vec3 expected = {
    -0.5746f,
    -0.3304f,
    -0.7488f
  };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST(
    "obj_float_line_to_vector should convert an obj vt line\n"
    "into a Vec2 of floats"
  );
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "vt 0.625000 0.750000";
  Vec2 actual = {0};
  obj_float_line_to_vector(str, &actual.x);
  Vec2 expected = { 0.625000f, 0.750000f };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    2,
    &f_tol
  ));
  PASSED

  TEST(
    "obj_f_line_to_3_ui_indices should convert the obj f string\n"
    "into a Vec3 of indices; remember i-1"
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
    "obj_f_triplet_line_to_vec3s should convert the "
    "str into 3 corresponding vectors; remember i-1"
  );
  f_tol.tolerance = FLT_EPSILON;
  const char *str = "f 10/6/5 11/3/5 9/12/5";
  Vec3 actual_vi = {0};
  Vec3 actual_vni = {0};
  Vec3 actual_vti = {0};
  obj_f_triplet_line_to_vec3s(
    str,
    &actual_vi.x,
    &actual_vni.x,
    &actual_vti.x
  );
  Vec3 expected_vi = { 9, 10, 8 };
  Vec3 expected_vni = { 4, 4, 4 };
  Vec3 expected_vti = { 5, 2, 11 };
  assert(vec_equals_vec(
    &actual_vi.x,
    &expected_vi.x,
    3,
    &f_tol
  ));
  assert(vec_equals_vec(
    &actual_vni.x,
    &expected_vni.x,
    3,
    &f_tol
  ));
  assert(vec_equals_vec(
    &actual_vti.x,
    &expected_vti.x,
    3,
    &f_tol
  ));
  PASSED

  TEST(
    "calculate_face_normal should calculate a normalized\n"
    "normal for the provided triangle of positions"
  );
  f_tol.tolerance = FLT_EPSILON;
  Vec3 positions[3] = {
    { -1, 0, 0 },
    { 0, 0, 1 },
    { 0, 1, 0 }
  };
  Vec3 actual = calculate_face_normal(positions);
  Vec3 expected = {
    -0.577350269f,
    0.577350269f,
    0.577350269f
  };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("calculate_face_normal run #2");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 positions[3] = {
    { 0, 0, 0 },
    { 0, 0, -1 },
    { 0, 1, 0 }
  };
  Vec3 actual = calculate_face_normal(positions);  
  Vec3 expected = { 1, 0, 0 };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST(
    "calculate_vertex_normal should calculate a\n"
    "normalized normal for the given vert index"
  );
  f_tol.tolerance = FLT_EPSILON;
  Vec3 actual = calculate_vertex_normal(
    0,
    36,
    cube_indices,
    cube_vertex_positions
  );
  Vec3 expected = {
    0.577350269f,
    0.577350269f,
    -0.577350269f,
  };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("calculate_vertex_normal run #2");
  f_tol.tolerance = FLT_EPSILON;
  Vec3 actual = calculate_vertex_normal(
    4,
    18,
    triangular_pyramid_indices,
    triangular_pyramid_positions
  );
  Vec3 expected = { 0, 1, 0 };
  assert(vec_equals_vec(
    &actual.x,
    &expected.x,
    3,
    &f_tol
  ));
  PASSED

  TEST("obj_shading_is_flat should return 0 if the obj file s line value is 1");
  FILE *obj_file = fopen("test_data/quad.obj", "r");
  assert(obj_shading_is_flat(obj_file) == 0);
  PASSED

  TEST("obj_shading_is_flat should return 1 if the obj file s line value is 0");
  FILE *obj_file = fopen("test_data/flat_pyramid.obj", "r");
  assert(obj_shading_is_flat(obj_file) == 1);
  PASSED

  TEST(
    "parse_obj_into_smooth_mesh should process a smooth formatted quad obj\n"
    "file into vertex and index arrays"
  );
  f_tol.tolerance = FLT_EPSILON;
  Vertex actual_vertices[4] = {0};
  unsigned int actual_indices[4] = {0};
  int actual_vertex_count = 0;
  int actual_index_count = 0;
  FILE *obj_file = fopen("test_data/quad.obj", "r");
  parse_obj_into_smooth_mesh(
    obj_file,
    actual_vertices,
    actual_indices,
    &actual_vertex_count,
    &actual_index_count
  );
  fclose(obj_file);
  Vertex expected_vertices[4] = {
    {{ -1, 0, 1 }, { 0, 1, 0 }, { 0, 0 }},
    {{ 1, 0, 1 }, { 0, 1, 0 }, { 1, 0 }},
    {{ -1, 0, -1 }, { 0, 1, 0 }, { 0, 1 }},
    {{ 1, 0, -1 }, { 0, 1, 0 }, { 1, 1 }}
  };
  for (int i = 0; i < 4; i++)
    assert(vert_equals_vert(
      &expected_vertices[i],
      &actual_vertices[i],
      &f_tol
    ));
  unsigned int expected_indices[18] = {
    1, 2, 0,
    1, 3, 2
  };
  for (int i = 0; i < 6; i++) assert(expected_indices[i] == actual_indices[i]);
  PASSED

  TEST(
    "parse_obj_into_flat_mesh should process a flat formatted pyramid obj\n"
    "file into vertex and idnex arrays"
  );
  f_tol.tolerance = FLT_EPSILON;
  Vertex actual_vertices[18] = {0};
  unsigned int actual_indices[18] = {0};
  int actual_vertex_count = 0;
  int actual_index_count = 0;
  FILE *obj_file = fopen("test_data/flat_pyramid.obj", "r");
  parse_obj_into_flat_mesh(
    obj_file,
    actual_vertices,
    actual_indices,
    &actual_vertex_count,
    &actual_index_count
  );
  fclose(obj_file);
  Vertex expected_vertices[18] = {
    {{ -0.5f, 0, -0.5f }, { -0.0000f, 0.7071f, -0.7071f }, { 0.250000f, 0.490000f }}, // 1
    {{ 0, 0.5f, 0 },{ -0.0000f, 0.7071f, -0.7071f },{ 0.250000f, 0.250000f }},
    {{ 0.5f, 0, -0.5f },{ -0.0000f, 0.7071f, -0.7071f },{ 0.490000f, 0.250000f }},
    {{ 0.5f, 0, -0.5f },{ 0.7071f, 0.7071f, -0.0000f },{ 0.490000f, 0.250000f }}, // 2
    {{ 0, 0.5f, 0 },{ 0.7071f, 0.7071f, -0.0000f },{ 0.250000f, 0.250000f }},
    {{ 0.5f, 0, 0.5f },{ 0.7071f, 0.7071f, -0.0000f },{ 0.250000f, 0.010000f }},
    {{ 0.5f, 0, -0.5f },{ -0.0000f, -1.0000f, -0.0000f },{ 0.990000f, 0.250000f }}, // 3
    {{ -0.5f, 0, 0.5f },{ -0.0000f, -1.0000f, -0.0000f },{ 0.510000f, 0.250000f }},
    {{ -0.5f, 0, -0.5f },{ -0.0000f, -1.0000f, -0.0000f },{ 0.750000f, 0.490000f }},
    {{ 0.5f, 0, 0.5f },{ -0.0000f, 0.7071f, 0.7071f },{ 0.250000f, 0.010000f }}, // 4
    {{ 0, 0.5f, 0 },{ -0.0000f, 0.7071f, 0.7071f },{ 0.250000f, 0.250000f }},
    {{ -0.5f, 0, 0.5f },{ -0.0000f, 0.7071f, 0.7071f },{ 0.010000f, 0.250000f }},
    {{ -0.5f, 0, 0.5f },{ -0.7071f, 0.7071f, -0.0000f },{ 0.010000f, 0.250000f }}, // 5
    {{ 0, 0.5f, 0 },{ -0.7071f, 0.7071f, -0.0000f },{ 0.250000f, 0.250000f }},
    {{ -0.5, 0, -0.5f },{ -0.7071f, 0.7071f, -0.0000f },{ 0.250000f, 0.490000f }},
    {{ 0.5f, 0, -0.5f },{ -0.0000f, -1.0000f, -0.0000f },{ 0.990000f, 0.250000f }}, // 6
    {{ 0.5f, 0, 0.5f },{ -0.0000f, -1.0000f, -0.0000f },{ 0.750000f, 0.010000f }},
    {{ -0.5, 0, 0.5 },{ -0.0000f, -1.0000f, -0.0000f },{ 0.510000f, 0.250000f }}
  };
  unsigned int expected_indices[18] = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
    12, 13, 14,
    15, 16, 17
  };
  for (int i = 0; i < 18; i++) {
    assert(vert_equals_vert(
      &expected_vertices[i],
      &actual_vertices[i],
      &f_tol
    ));
    assert(expected_indices[i] == actual_indices[i]);
  }
  PASSED
  
  printf("\n\n");
  printf("_____________________________________\n");
  printf("_________ALL_TESTS_PASSED_:)_________\n");
  printf("_____________________________________\n");
  
  return 0;
}