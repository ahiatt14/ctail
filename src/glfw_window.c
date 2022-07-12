#include <stdint.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#include "window.h"
#include "vector.h"

// TODO: add fullscreen/windowed toggling
// TODO: handle monitor connecting/disconnecting

static GLFWwindow *glfw_window;

void (*handle_window_minimize)();
void (*handle_window_restore)();
void (*handle_window_focus)();
void (*handle_window_unfocus)();
void (*handle_framebuffer_resize)(uint16_t width, uint16_t height);
void (*handle_joystick_connected)(int jid);
void (*handle_joystick_disconnected)(int jid);

uint8_t received_closed_event() {
  return glfwWindowShouldClose(glfw_window);
}

void request_buffer_swap() {
  glfwSwapBuffers(glfw_window);
}

void poll_events() {
  glfwPollEvents();
}

void end() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

uint8_t gamepad_is_connected() {
  return glfwJoystickPresent(GLFW_JOYSTICK_1);
}

static void handle_glfw_framebuffer_resize(
  GLFWwindow *w,
  int width,
  int height
) {
  handle_framebuffer_resize(width, height);
}

static void handle_window_focus_change(GLFWwindow *w, int gained_focus) {
  gained_focus ? handle_window_focus() : handle_window_unfocus();
}

static void handle_window_iconification(GLFWwindow *w, int is_minimized) {
  is_minimized ? handle_window_minimize() : handle_window_restore();
}

static void handle_joystick_connection_event(int jid, int event) {
  if (event == GLFW_CONNECTED) {
    handle_joystick_connected(jid);
  } else if (event == GLFW_DISCONNECTED) {
    handle_joystick_disconnected(jid);
  }
}

static struct vec2 get_window_dimensions() {
  int width, height;
  glfwGetWindowSize(glfw_window, &width, &height);
  return (struct vec2){ width, height };
  // TODO: will this struct return by value or poof?
  // prolly will have to pass a destination vec2 ptr
}

static void get_gamepad_input(struct gamepad_input *const input) {
  GLFWgamepadstate state;
  if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
    *input = (struct gamepad_input){0};
    return;
  }
  // TODO: include d-pad direction
  input->left_stick_direction = (struct vec2){
    .x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X],
    .y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]
  };
  input->start_down = state.buttons[GLFW_GAMEPAD_BUTTON_START];
  input->select_down = state.buttons[GLFW_GAMEPAD_BUTTON_BACK];
  input->right_trigger = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
  input->left_trigger = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
  input->top_face_down = state.buttons[GLFW_GAMEPAD_BUTTON_Y];
  input->bottom_face_down = state.buttons[GLFW_GAMEPAD_BUTTON_A];
  input->right_face_down = state.buttons[GLFW_GAMEPAD_BUTTON_B];
  input->left_face_down = state.buttons[GLFW_GAMEPAD_BUTTON_X];
}

static void on_minimize_and_restore(
  void (*handle_minimize)(),
  void (*handle_restore)()
) {
  handle_window_minimize = handle_minimize;
  handle_window_restore = handle_restore;
  glfwSetWindowIconifyCallback(glfw_window, handle_window_iconification);
}

static void on_focus_and_unfocus(
  void (*handle_focus)(),
  void (*handle_unfocus)()
) {
  handle_window_unfocus = handle_unfocus;
  handle_window_focus = handle_focus;
  glfwSetWindowFocusCallback(glfw_window, handle_window_focus_change);
}

static void on_framebuffer_resize(
  void (*fn)(uint16_t width, uint16_t height)
) {
  handle_framebuffer_resize = fn;
  glfwSetFramebufferSizeCallback(glfw_window, handle_glfw_framebuffer_resize);
}

static void on_gamepad_connect_and_disconnect(
  void (*handle_gamepad_connect)(int jid),
  void (*handle_gamepad_disconnect)(int jid)
) {
  handle_joystick_connected = handle_gamepad_connect;
  handle_joystick_disconnected = handle_gamepad_disconnect;
  glfwSetJoystickCallback(handle_joystick_connection_event);
}

static double get_seconds_since_creation() {
  return glfwGetTime();
}

uint8_t window__create(
  uint16_t window_width,
  uint16_t window_height,
  uint16_t position_x,
  uint16_t position_y,
  const char *name,
  uint8_t vsync,
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

  glfwSetWindowPos(glfw_window, position_x, position_y); // see note A
  glfwShowWindow(glfw_window); // see note A

  glfwSetWindowRefreshCallback(glfw_window, glfwSwapBuffers);

  glfwMakeContextCurrent(glfw_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 0;

  glfwSwapInterval(vsync);

  window->on_minimize_and_restore = on_minimize_and_restore;
  window->on_focus_and_unfocus = on_focus_and_unfocus;
  window->on_framebuffer_resize = on_framebuffer_resize;
  window->on_gamepad_connect_and_disconnect = on_gamepad_connect_and_disconnect;

  window->get_gamepad_input = get_gamepad_input;
  window->gamepad_is_connected = gamepad_is_connected;

  window->get_window_dimensions = get_window_dimensions;
  window->get_seconds_since_creation = get_seconds_since_creation;
  window->request_buffer_swap = request_buffer_swap;
  
  window->poll_events = poll_events;
  window->received_closed_event = received_closed_event;
  window->end = end;

  return 1;
}

/*

---------- NOTES ----------

A) if manually setting the position of a new window (with glfwSetWindowPos)
after its creation, it may appear to blink. To prevent this, we want to hide the
window with a hint before its creation, then set its size, then call
glfwShowWindow(GLFWwindow*)

*/