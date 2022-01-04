#include <string.h>

#include "worldspace.h"
#include "tail_math.h"
#include "public_types.h"
#include "m4x4.h"

static vec4 world_up;
static vec4 world_right;
static vec4 world_forward;

static m4x4 temp__model_scale;
static m4x4 temp__model_translation;
static m4x4 temp__model_y_rotation;

void worldspace__set_world_axes(
  const vec4 *up,
  const vec4 *right,
  const vec4 *forward
) {
  memcpy(&world_up.x, &up->x, sizeof(vec4));
  memcpy(&world_right.x, &right->x, sizeof(vec4));
  memcpy(&world_forward.x, &forward->x, sizeof(vec4));
}

const vec4* worldspace__get_up() {
  return &world_up;
}
const vec4* worldspace__get_right() {
  return &world_right;
}
const vec4* worldspace__get_forward() {
  return &world_forward;
}

void worldspace__default_transform(transform *t) {
  m4x4_identity(&t->model);
  t->scale = 1.0f;
}

void worldspace__calculate_model(transform *t) {

  m4x4_identity(&t->model);
  m4x4_translation(&t->position, &temp__model_translation);
  m4x4_scaling(t->scale, &temp__model_scale);

  m4x4_rotation(
    deg_to_rad(t->rotation.y),
    &world_up,
    &temp__model_y_rotation
  );

  // m4x4_x_m4x4(&t->model, &temp__model_scale, &t->model);
  // m4x4_x_m4x4(&t->model, &temp__model_y_rotation, &t->model);
  m4x4_x_m4x4(&t->model, &temp__model_translation, &t->model);
}