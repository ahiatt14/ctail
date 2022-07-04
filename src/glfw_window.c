#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#include "window.h"

// TODO: add fullscreen/windowed toggling
// TODO: handle monitor connecting/disconnecting

static GLFWwindow *glfw_window;

typedef void (*handle_window_minimize_ptr)();
typedef void (*handle_window_restore_ptr)();
typedef void (*handle_window_focus_ptr)();
typedef void (*handle_window_unfocus_ptr)();
typedef void (*handle_window_resize_ptr)(int width, int height);
handle_window_minimize_ptr handle_window_minimize;
handle_window_restore_ptr handle_window_restore;
handle_window_focus_ptr handle_window_focus;
handle_window_unfocus_ptr handle_window_unfocus;
handle_window_resize_ptr handle_window_resize;

unsigned short int window__received_closed_event() {
  return glfwWindowShouldClose(glfw_window);
}

void request_buffer_swap() {
  glfwSwapBuffers(glfw_window);
}

void window__poll_events() {
  glfwPollEvents();
}

void window__end() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

static void handle_framebuffer_resize(GLFWwindow *w, int width, int height) {
  handle_window_resize(width, height);
}

static void handle_window_focus_change(GLFWwindow *w, int gained_focus) {
  gained_focus ? handle_window_focus() : handle_window_unfocus();
}

static void handle_window_iconification(GLFWwindow *w, int is_minimized) {
  is_minimized ? handle_window_minimize() : handle_window_restore();
}

static void register_listener_for_minimize(
  void (*handle_minimize)(),
  void (*handle_restore)()
) {
  handle_window_minimize = handle_minimize;
  handle_window_restore = handle_restore;
  glfwSetWindowIconifyCallback(glfw_window, handle_window_iconification);
}

static void register_listener_for_focus(
  void (*handle_focus)(),
  void (*handle_unfocus)()
) {
  handle_window_unfocus = handle_unfocus;
  handle_window_focus = handle_focus;
  glfwSetWindowFocusCallback(glfw_window, handle_window_focus_change);
}

static void register_listener_for_resize(void (*fn)(int width, int height)) {
  handle_window_resize = fn;
  glfwSetFramebufferSizeCallback(glfw_window, handle_framebuffer_resize);
}

static double get_seconds_since_creation() {
  return glfwGetTime();
}

unsigned short int window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync,
  struct window_api *const window
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

  glfw_window = glfwCreateWindow(
    window_width,
    window_height,
    name,
    NULL, // monitor handle. NOTE: forces fullscreen mode
    NULL // context obj sharing
  );

  if (!glfw_window) return 0;

  glfwSetWindowPos(glfw_window, 100, 100); // see note A
  glfwShowWindow(glfw_window); // see note A

  glfwSetWindowRefreshCallback(glfw_window, glfwSwapBuffers);

  glfwMakeContextCurrent(glfw_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 0;

  glfwSwapInterval(vsync);

  window->get_seconds_since_creation = get_seconds_since_creation;
  window->register_listener_for_minimize = register_listener_for_minimize;
  window->register_listener_for_focus = register_listener_for_focus;
  window->register_listener_for_resize = register_listener_for_resize;
  window->request_buffer_swap = request_buffer_swap;

  return 1;
}

/*

---------- NOTES ----------

A) if manually setting the position of a new window (with glfwSetWindowPos)
after its creation, it may appear to blink. To prevent this, we want to hide the
window with a hint before its creation, then set its size, then call
glfwShowWindow(GLFWwindow*)

*/