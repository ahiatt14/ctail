#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "tail.h"

#define TEST(msg) printf(msg)
#define PASSED printf(": PASSED\n");

#define EPSILON 0.000000000000000000000000000000000000001f
// TODO: wait what? tests should be failing with an epsilon this small?

int floats_are_close_enough(float a, float b);
int m4x4_equals_m4x4(const m4x4 *m0, const m4x4 *m1);
void print_m4x4(const char *name, const m4x4 *m);

int main(void) {

  vec4 actual_v;
  m4x4 expected_m, actual_m;

  TEST("vec4_point should create a vec4 with a w component of 1.0");
  vec4_point(2, 4.05f, 40, &actual_v);
  assert(floats_are_close_enough(2.0f, actual_v.x));
  assert(floats_are_close_enough(4.05f, actual_v.y));
  assert(floats_are_close_enough(40, actual_v.z));
  assert(floats_are_close_enough(1, actual_v.w));
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
    1, 6, 7, 3, 1, 6, 7, 3, 0, 0, 1, 4, 0, 0, 1, 5
  };
  memcpy(&expected_m.data, &expected_m_data[0], sizeof(expected_m_data));
  assert(m4x4_equals_m4x4(&expected_m, &actual_m));
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
  assert(m4x4_equals_m4x4(&expected_m, &actual_m));
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
  assert(m4x4_equals_m4x4(&expected_m, &actual_m));
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
  assert(m4x4_equals_m4x4(&expected_m, &actual_m));
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
  assert(m4x4_equals_m4x4(&expected_m, &actual_m));
  PASSED

  // TEST("m4x4_rotation should create a rotation matrix");

  // PASSED
}

void print_m4x4(const char *name, const m4x4 *m) {
  const float *data = &(m->data[0]);
  printf("%s\n", name);
  for (int i = 0; i < 16; i++) {
    printf("%f, ", data[i]);
    if (
      i == 3 ||
      i == 7 ||
      i == 11
    ) printf("\n");
  }
  printf("\n");
}

int m4x4_equals_m4x4(
  const m4x4 *m0,
  const m4x4 *m1
) {
  for (int i = 0; i < 16; i++) {
    if (!floats_are_close_enough(m0->data[i], m1->data[i])) return 0;
  }
  return 1;
}

int floats_are_close_enough(float a, float b) {
  return fabs(a - b) < EPSILON ? 1 : 0;
}