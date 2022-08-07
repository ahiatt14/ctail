#include <stdint.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#include "window.h"
#include "vector.h"
#include "gamepad.h"

static uint8_t GLFW_BUTTON_COUNT = 15;

// TODO: handle monitor connecting/disconnecting

struct window_props {
  int width_in_screen_units;
  int height_in_screen_units;
  int position_x;
  int position_y;
};

// CACHE

static GLFWwindow *glfw_window;

struct window_props win_props;

static void (*handle_window_minimize)();
static void (*handle_window_restore)();
static void (*handle_window_focus)();
static void (*handle_window_unfocus)();
static void (*handle_framebuffer_resize)(uint16_t width, uint16_t height);
static void (*handle_joystick_connected)(int jid);
static void (*handle_joystick_disconnected)(int jid);

static void cache_window_properties() {
  glfwGetWindowSize(
    glfw_window,
    &win_props.width_in_screen_units,
    &win_props.height_in_screen_units
  );
  glfwGetWindowPos(
    glfw_window,
    &win_props.position_x,
    &win_props.position_y
  );
}

/*
  EVENTS AND HANDLERS
*/

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

/*
  GETTING AND SETTING WINDOW PROPERTIES
*/

uint8_t gamepad_is_connected() {
  return glfwJoystickPresent(GLFW_JOYSTICK_1);
}

static struct vec2 get_window_dimensions() {
  int width, height;
  glfwGetWindowSize(glfw_window, &width, &height);
  return (struct vec2){ width, height };
}

static void get_gamepad_input(struct gamepad_input *const gamepad) {

  gamepad->previous_buttons = gamepad->buttons;

  static GLFWgamepadstate state;
  if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
    gamepad->buttons = 0;
    gamepad->previous_buttons = 0;
    return;
  }

  gamepad->left_stick_direction = (struct vec2){
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

static void switch_to_fullscreen() {
  cache_window_properties();
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  GLFWvidmode const *video_mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(
    glfw_window,
    monitor,
    0,
    0,
    video_mode->width,
    video_mode->height,
    video_mode->refreshRate // TODO: doesn't appear to work???
  );
}

static void switch_to_windowed() {
  glfwSetWindowMonitor(
    glfw_window,
    NULL,
    // TODO: these props won't be set if we start an app in fullscreen,
    // work for future projects
    win_props.position_x,
    win_props.position_y,
    win_props.width_in_screen_units,
    win_props.height_in_screen_units,
    0
  );
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

void end() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

uint8_t window__create(
  uint16_t win_width,
  uint16_t win_height,
  uint16_t pos_x,
  uint16_t pos_y,
  const char *name,
  uint8_t vsync,
  uint8_t fullscreen,
  uint8_t request_MSAA,
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

  if (request_MSAA) glfwWindowHint(GLFW_SAMPLES, 4);

  glfw_window = glfwCreateWindow(
    win_width,
    win_height,
    name,
    NULL, // monitor handle. NOTE: passing a montior forces fullscreen mode
    NULL // context obj sharing
  );

  if (!glfw_window) return 0;

  glfwSetWindowPos(glfw_window, pos_x, pos_y); // see note A
  glfwShowWindow(glfw_window); // see note A

  glfwSetWindowRefreshCallback(glfw_window, glfwSwapBuffers);

  glfwMakeContextCurrent(glfw_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 0;

  glfwSwapInterval(vsync);

  window->on_minimize_and_restore = on_minimize_and_restore;
  window->on_focus_and_unfocus = on_focus_and_unfocus;
  window->on_framebuffer_resize = on_framebuffer_resize;
  window->on_gamepad_connect_and_disconnect = on_gamepad_connect_and_disconnect;

  window->gamepad_is_connected = gamepad_is_connected;
  window->get_gamepad_input = get_gamepad_input;

  window->is_fullscreen = is_fullscreen;
  window->get_window_dimensions = get_window_dimensions;
  window->get_seconds_since_creation = get_seconds_since_creation;
  window->switch_to_fullscreen = switch_to_fullscreen;
  window->switch_to_windowed = switch_to_windowed;

  window->poll_events = poll_events;
  window->request_buffer_swap = request_buffer_swap;
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