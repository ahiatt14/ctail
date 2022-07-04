#include "viewport.h"

void viewport__set_width(int w, struct viewport *v) {
  v->_width = w;
  v->_perspective_needs_recalculating = 1;
}
void viewport__set_height(int h, struct viewport *v) {
  v->_height = h;
  v->_perspective_needs_recalculating = 1;
}
float viewport__get_aspect_ratio(struct viewport const *const v) {
  return v->_width / v->_height;
}
short int viewport__perspective_needs_recalculating(
  struct viewport const *const v
) {
  return v->_perspective_needs_recalculating;
}