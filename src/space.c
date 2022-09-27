#include <string.h>

#include "space.h"
#include "tail_math.h"
#include "quaternion.h"
#include "m4x4.h"
#include "m3x3.h"

// TODO: more efficient caching? we'll be performing create_model a lot
static struct m4x4 temp__scale;
static struct m4x4 temp__translation;

static struct m4x4 temp__x_rotation;
static struct m4x4 temp__y_rotation;
static struct m4x4 temp__z_rotation;
static struct m4x4 temp__zx_rotation;
static struct m4x4 temp__zxy_rotation;

static struct m4x4 temp__rotate_and_scale;

void space__create_model(
  struct coordinate_space const *const space,
  struct transform const *const t,
  struct m4x4 *const dest
) {

  m4x4__identity(dest);
  
  m4x4__translation(t->position, &temp__translation);
  m4x4__scaling(t->scale, &temp__scale);

  m4x4__rotation(
    deg_to_rad(t->rotation_in_deg.x),
    space->right,
    &temp__x_rotation
  );
  m4x4__rotation(
    deg_to_rad(t->rotation_in_deg.y),
    space->up,
    &temp__y_rotation 
  );
  m4x4__rotation(
    deg_to_rad(t->rotation_in_deg.z),
    space->forward,
    &temp__z_rotation 
  );
  m4x4_x_m4x4(
    &temp__x_rotation,
    &temp__z_rotation,
    &temp__zx_rotation
  );
  m4x4_x_m4x4(
    &temp__y_rotation,
    &temp__zx_rotation,
    &temp__zxy_rotation
  );

  m4x4_x_m4x4(
    &temp__zxy_rotation,
    &temp__scale,
    &temp__rotate_and_scale
  );
  m4x4_x_m4x4(
    &temp__translation,
    &temp__rotate_and_scale,
    dest
  );
}

void space__create_normals_model(
  struct m4x4 const *const model,
  struct m3x3 *const dest
) {
  m4x4__sub3x3_from00(model, dest);
  m3x3__inverse(dest, dest);
  m3x3__transpose(dest, dest);
}