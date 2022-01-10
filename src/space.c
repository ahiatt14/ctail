#include <string.h>

#include "space.h"
#include "tail_math.h"
#include "public_types.h"
#include "m4x4.h"

// static m4x4 temp__model_scale;
// static m4x4 temp__model_translation;
// static m4x4 temp__model_y_rotation;

// void space__model_transform(
//   const coordinate_space *space,
//   const transform *t,
//   m4x4 *model
// ) {

//   m4x4_identity(model);
  
//   m4x4_translation(&t->position, &temp__model_translation);
//   m4x4_scaling(t->scale, &temp__model_scale);
//   m4x4_rotation(
//     deg_to_rad(t->rotation.y),
//     &space->up,
//     &temp__model_y_rotation
//   );

//   m4x4_x_m4x4(model, &temp__model_rotation, model);
//   m4x4_x_m4x4(model, &temp__model_scale, model);
//   m4x4_x_m4x4(model, &temp__model_translation, model);
// }