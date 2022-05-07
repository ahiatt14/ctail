#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

unsigned short int window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync
  // unsigned short int fullscreen, TODO: add support
);

unsigned short int window__received_closed_event();

double window__get_seconds_since_game_start();

void window__register_listener_for_minimize(void (*fn)());
void window__register_listener_for_restore(void (*fn)());
void window__register_listener_for_unfocus(void (*fn)());
void window__register_listener_for_focus(void (*fn)());
void window__register_listener_for_resize(void (*fn)(int width, int height));

double window__get_seconds_since_game_start();

void window__poll_events();
void window__end();

#endif