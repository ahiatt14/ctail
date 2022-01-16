#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "public_types.h"

void camera__init(camera *c);
const m4x4* camera__calculate_lookat(const vec4 *world_up, camera *c);
// const m4x4* camera__calculate_ortho_projection(
//   const viewport *vwprt,
//   camera *cmr
// );
const m4x4* camera__get_lookat(const camera *c);
const m4x4* camera__get_projection(const camera *c);
const vec4* camera__get_position(camera *c);
const vec4* camera__get_look_target(camera *c);
void camera__set_position(float x, float y, float z, camera *c);
void camera__set_look_target(const vec4 *t, camera *c);
int camera__lookat_needs_recalculating(const camera *c);

#endif