#ifndef __TAIL_WORLDSPACE__
#define __TAIL_WORLDSPACE__

#include "public_types.h"

void worldspace__set_world_axes_to_default();
const vec4* worldspace__get_up();
const vec4* worldspace__get_right();
const vec4* worldspace__get_forward();

void worldspace__default_transform(transform *t);

void worldspace__clear_transforms_to_remodel();
void worldspace__select_for_remodeling(transform *t);
void worldspace__remodel_selected_transforms();

void worldspace__calculate_model(transform *t);

#endif