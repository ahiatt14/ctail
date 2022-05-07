#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "window.h"

static GLFWwindow *window;

typedef void (*on_window_minimize_ptr)();
typedef void (*on_window_restore_ptr)();
typedef void (*on_window_unfocus_ptr)();
typedef void (*on_window_focus_ptr)();
typedef void (*on_window_resize_ptr)();
on_window_minimize_ptr handle_window_minimize;
on_window_restore_ptr handle_window_restore;
on_window_unfocus_ptr handle_window_unfocus;
on_window_focus_ptr handle_window_focus;
on_window_resize_ptr handle_window_resize;

// TODO: add fullscreen/windowed toggling
// TODO: handle monitor connecting/disconnecting
static void handle_framebuffer_resize(
  GLFWwindow *window, int pixel_w, int pixel_h
);
static void handle_window_focus_change(
  GLFWwindow *window, int gained_focus
);
static void handle_window_iconification(
  GLFWwindow *window, int was_iconified
);

unsigned short int window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync
) {

  if (!glfwInit()) return 0;

  // GLFW GL CONTEXT HINTS
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW WINDOW HINTS
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // see note A
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);

  window = glfwCreateWindow(
    window_width,
    window_height,
    "fool",
    NULL, // monitor handle. NOTE: forces fullscreen mode
    NULL // context obj sharing
  );

  if (!window) return 0;

  glfwSetWindowPos(window, 100, 100); // see note A
  glfwShowWindow(window); // see note A

  glfwSetWindowRefreshCallback(window, glfwSwapBuffers);

  glfwMakeContextCurrent(window);

  glfwSwapInterval(vsync);

  return 1;
}

unsigned short int window__received_closed_event() {
  return glfwWindowShouldClose(window);
}

void window__register_listener_for_minimize(void (*fn)()) {
  handle_window_minimize = fn;
  glfwSetWindowIconifyCallback(window, handle_window_iconification);
}

void window__register_listener_for_restore(void (*fn)()) {
  handle_window_restore = fn;
}

void window__register_listener_for_unfocus(void (*fn)()) {
  handle_window_unfocus = fn;
  glfwSetWindowFocusCallback(window, handle_window_focus_change);
  // TODO: no good.
  // either provide 2 callbacks for a single listener and split in here
  // or make a single callback that returns 0/1 for unfocus/focus for client
  // to handle. Same goes for resize & iconify
}

void window__register_listener_for_focus(void (*fn)()) {
  handle_window_focus = fn;
  glfwSetWindowFocusCallback(window, handle_window_focus_change);
}

void window__register_listener_for_resize(void (*fn)()) {
  handle_window_resize = fn;
  glfwSetFramebufferSizeCallback(window, handle_framebuffer_resize);
}

void handle_framebuffer_resize(GLFWwindow *window, int w, int h) {
  handle_window_resize(w, h);
}

void handle_window_focus_change(GLFWwindow *window, int gained_focus) {
  gained_focus ? handle_window_focus() : handle_window_unfocus();
}

void handle_window_iconification(GLFWwindow *window, int is_minimized) {
  is_minimized ? handle_window_minimize() : handle_window_restore();
}

double window__get_seconds_since_game_start() {
  return glfwGetTime();
}

void window__end() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

/*

---------- NOTES ----------

A) if manually setting the position of a new window (with glfwSetWindowPos)
after its creation, it may appear to blink. To prevent this, we want to hide the
window with a hint before its creation, then set its size, then call
glfwShowWindow(GLFWwindow*)

*/