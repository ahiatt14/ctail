#include <stdint.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#include "window.h"
#include "vector.h"
#include "gamepad.h"

/*
  CONSTANTS
*/

static uint8_t GLFW_BUTTON_COUNT = 15;

/*
  LOCALS
*/

static GLFWwindow *glfw_window;

static uint8_t vsync_requested;

/*
  EVENTS AND HANDLERS
*/

static void (*handle_window_minimize)();
static void (*handle_window_restore)();
static void (*handle_window_focus)();
static void (*handle_window_unfocus)();
static void (*handle_framebuffer_resize)(uint16_t width, uint16_t height);
static void (*handle_joystick_connected)(int jid);
static void (*handle_joystick_disconnected)(int jid);

static void handle_glfw_framebuffer_resize(
  GLFWwindow *w,
  int width_in_pixels,
  int height_in_pixels
) {
  handle_framebuffer_resize(width_in_pixels, height_in_pixels);
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
  void (*fn)(uint16_t width_in_pixels, uint16_t height_in_pixels)
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

/*
  GETTING AND SETTING WINDOW PROPERTIES
*/

uint8_t gamepad_is_connected() {
  return glfwJoystickPresent(GLFW_JOYSTICK_1);
}

static Vec2 get_window_dim_in_screen_units() {
  int width, height;
  glfwGetWindowSize(glfw_window, &width, &height);
  return (Vec2){ width, height };
}

static Vec2 get_framebuffer_size() {
  int width, height;
  glfwGetFramebufferSize(glfw_window, &width, &height);
  return (Vec2){ width, height };
}

static void get_gamepad_input(Gamepad *const gamepad) {

  gamepad->previous_buttons = gamepad->buttons;

  static GLFWgamepadstate state;
  if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
    gamepad->buttons = 0;
    gamepad->previous_buttons = 0;
    return;
  }

  gamepad->left_stick_direction = (Vec2){
    .x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X],
    .y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]
  };

  gamepad->left_trigger = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
  gamepad->right_trigger = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
  
  for (int button = 0; button < GLFW_BUTTON_COUNT; button++) {
    if (state.buttons[button] == GLFW_PRESS) {
      gamepad->buttons |= 1 << button;
    } else {
      gamepad->buttons &= ~(1 << button);
    }
  }
}

static double get_seconds_since_creation() {
  return glfwGetTime();
}

static uint8_t is_fullscreen() {
  return (glfwGetWindowMonitor(glfw_window) != NULL) ? 1 : 0;
}

static void enable_vsync() {
  vsync_requested = REQUEST_VSYNC_ON;
  glfwSwapInterval(vsync_requested);
}

static void disable_vsync() {
  vsync_requested = REQUEST_VSYNC_OFF;
  glfwSwapInterval(vsync_requested);
}

static void switch_to_fullscreen() {
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  GLFWvidmode const *video_mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(
    glfw_window,
    monitor,
    0,
    0,
    video_mode->width,
    video_mode->height,
    video_mode->refreshRate
  );
  glfwSwapInterval(vsync_requested);
  glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

static void switch_to_windowed(
  uint16_t position_x,
  uint16_t position_y,
  uint16_t width_in_screen_units,
  uint16_t height_in_screen_units
) {
  glfwSetWindowMonitor(
    glfw_window,
    NULL,
    position_x,
    position_y,
    width_in_screen_units,
    height_in_screen_units,
    0
  );
  glfwSwapInterval(vsync_requested);
  glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/*
  WINDOW LIFECYCLE AND LOOP
*/

uint8_t received_closed_event() {
  return glfwWindowShouldClose(glfw_window);
}

void request_buffer_swap() {
  glfwSwapBuffers(glfw_window);
}

void poll_events() {
  glfwPollEvents();
}

void wait_on_events() {
  glfwWaitEvents();
}

void end() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

/*
  WINDOW CREATION
*/

static void set_openGL_version_hints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static uint8_t connect_openGL(
  GLFWwindow *const glfw_window
) {
  glfwMakeContextCurrent(glfw_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 0;
  return 1;
}

static void create_PC_window_api(
  Window *const window
) {
  window->on_minimize_and_restore = on_minimize_and_restore;
  window->on_focus_and_unfocus = on_focus_and_unfocus;
  window->on_framebuffer_resize = on_framebuffer_resize;
  window->on_gamepad_connect_and_disconnect = on_gamepad_connect_and_disconnect;

  window->gamepad_is_connected = gamepad_is_connected;
  window->get_gamepad_input = get_gamepad_input;

  window->is_fullscreen = is_fullscreen;
  window->get_window_dim_in_screen_units = get_window_dim_in_screen_units;
  window->get_framebuffer_size = get_framebuffer_size;
  window->get_seconds_since_creation = get_seconds_since_creation;
  window->switch_to_fullscreen = switch_to_fullscreen;
  window->switch_to_windowed = switch_to_windowed;
  window->enable_vsync = enable_vsync;
  window->disable_vsync = disable_vsync;

  window->poll_events = poll_events;
  window->wait_on_events = wait_on_events;
  window->request_buffer_swap = request_buffer_swap;
  window->received_closed_event = received_closed_event;
  window->end = end;
}

uint8_t window__create_fullscreen_game(
  const char *name,
  uint8_t request_vsync,
  uint8_t MSAA_samples,
  Window *const window
) {

  if (!glfwInit()) return 0;

  set_openGL_version_hints();
  if (MSAA_samples > 0) glfwWindowHint(GLFW_SAMPLES, MSAA_samples);

  GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
  GLFWvidmode const *video_mode = glfwGetVideoMode(primary_monitor);

  glfw_window = glfwCreateWindow(
    video_mode->width,
    video_mode->height,
    name,
    primary_monitor,
    NULL
  );

  if (!glfw_window) return 0;

  vsync_requested = request_vsync;
  glfwSwapInterval(vsync_requested);
  glfwSetWindowRefreshCallback(glfw_window, glfwSwapBuffers);

  glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  if (!connect_openGL(glfw_window)) return 0;
  create_PC_window_api(window);

  return 1;
}

uint8_t window__create_windowed_game(
  uint16_t win_width_in_screen_coord,
  uint16_t win_height_in_screen_coord,
  uint16_t position_x,
  uint16_t position_y,
  const char *name,
  uint8_t request_vsync,
  uint8_t MSAA_samples,
  Window *const window
) {

  if (!glfwInit()) return 0;

  set_openGL_version_hints();
  if (MSAA_samples > 0) glfwWindowHint(GLFW_SAMPLES, MSAA_samples);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // see note A

  glfw_window = glfwCreateWindow(
    win_width_in_screen_coord,
    win_height_in_screen_coord,
    name,
    NULL,
    NULL
  );

  if (!glfw_window) return 0;

  vsync_requested = request_vsync;
  glfwSwapInterval(vsync_requested);

  glfwSetWindowPos(glfw_window, position_x, position_y); // see note A
  glfwShowWindow(glfw_window); // see note A
  glfwSetWindowRefreshCallback(glfw_window, glfwSwapBuffers);

  if (!connect_openGL(glfw_window)) return 0;
  create_PC_window_api(window);

  return 1;
}

/*

---------- NOTES ----------

A) if manually setting the position of a new window (with glfwSetWindowPos)
after its creation, it may appear to blink. To prevent this, we want to hide the
window with a hint before its creation, then set its size, then call
glfwShowWindow(GLFWwindow*)

*/