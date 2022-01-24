#include "viewport.h"
#include "public_types.h"

void viewport__set_width(int w, viewport *v) {
  v->_width = w;
  v->_perspective_needs_recalculating = 1;
}
void viewport__set_height(int h, viewport *v) {
  v->_height = h;
  v->_perspective_needs_recalculating = 1;
}
float viewport__get_aspect_ratio(const viewport *v) {
  return v->_width / v->_height;
}
short int viewport__perspective_needs_recalculating(const viewport *v) {
  return v->_perspective_needs_recalculating;
}