#ifndef __TAIL_VEC4__
#define __TAIL_VEC4__

#include "public_types.h"

void vec4_vector(float x, float y, float z, vec4 *t);
void vec4_point(float x, float y, float z, vec4 *t);
void vec4_up(vec4 *t);
float vec4_magnitude(const vec4 *t);
void vec4_normalize(vec4 *t);
float vec4_dot(const vec4 *t0, const vec4 *t1);
void vec4_cross(const vec4 *t0, const vec4 *t1, vec4 *prod);
void vec4_minus_vec4(const vec4 *t0, const vec4 *t1, vec4 *diff);
void m4x4_x_vec4(const m4x4 *m, const vec4 *t, vec4 *dest);

#endif