#include "tail.h"

struct window_api window;
struct gpu_api gpu;

int main() {

  if (!window__create(
    400,
    400,
    100,
    100
    "test",
    REQUEST_VSYNC_ON,
    REQUEST_WINDOWED,
    REQUEST_MSAA_OFF,
    &window
  )) return 1;

  while (!window__received_closed_event()) {
    window__poll_events();
  }
  
  window__end();
  return 0;
}