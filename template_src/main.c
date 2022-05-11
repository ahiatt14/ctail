#include "tail.h"

#define VSYNC_ON 1

int main() {

  const window_api *window = window__create(
    800,
    800,
    "test",
    VSYNC_ON
  );

  while (!window__received_closed_event()) {
    window__poll_events();
  }
  
  window__end();
  return 0;
}