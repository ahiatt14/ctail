#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

double window__get_seconds_since_creation();

unsigned short int window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync
  // unsigned short int fullscreen, TODO: add support
);

void window__register_listener_for_minimize(
  void (*handle_minimize)(),
  void (*handle_restore)()
);
void window__register_listeners_for_focus(
  void (*handle_focus)(),
  void (*handle_unfocus)()
);
void window__register_listener_for_resize(
  void (*fn)(int width, int height)
);

unsigned short int window__received_closed_event();
void window__poll_events();
void window__end();

#endif