#include "viewport.h"

void viewport__set_width(int w, struct Viewport *v) {
  v->_width = w;
  v->_perspective_needs_recalculating = 1;
}
void viewport__set_height(int h, struct Viewport *v) {
  v->_height = h;
  v->_perspective_needs_recalculating = 1;
}
float viewport__get_aspect_ratio(struct Viewport const *const v) {
  return v->_width / v->_height;
}
short int viewport__perspective_needs_recalculating(
  struct Viewport const *const v
) {
  return v->_perspective_needs_recalculating;
}