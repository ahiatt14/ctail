#include <string.h>

#include "space.h"
#include "tail_math.h"
#include "m4x4.h"

static m4x4 temp__model_scale;
static m4x4 temp__model_translation;
static m4x4 temp__model_z_rotation;

static m4x4 temp__rotate_x_scale;

// TODO: add quaternion-based rotation
void space__create_model(
  const coordinate_space *space,
  const transform *t,
  m4x4 *model
) {

  m4x4_identity(model);
  
  m4x4_translation(&t->position, &temp__model_translation);
  m4x4_scaling(t->scale, &temp__model_scale);
  m4x4_rotation(
    deg_to_rad(t->rotation_in_deg.z),
    &space->up,
    &temp__model_z_rotation
  );

  m4x4_x_m4x4(
    &temp__model_z_rotation,
    &temp__model_scale,
    &temp__rotate_x_scale
  );
  m4x4_x_m4x4(
    &temp__model_translation,
    &temp__rotate_x_scale,
    model
  );
}