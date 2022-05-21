#include "tail.h"

#define VSYNC_ON 1

struct window_api window;
struct gpu_api gpu;

int main() {

  if (!window__create(
    800,
    800,
    "test",
    VSYNC_ON,
    window
  )) return 1;

  while (!window__received_closed_event()) {
    window__poll_events();
  }
  
  window__end();
  return 0;
}