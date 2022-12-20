#include <stdint.h>

#include "gamepad.h"

uint8_t button_is_down(
  gamepad_button_id button,
  struct gamepad_input const *const gamepad
) {
  return (gamepad->buttons >> button) & 1;
}

uint8_t button_is_up(
  gamepad_button_id button,
  struct gamepad_input const *const gamepad
) {
  return button_is_down(button, gamepad) ? 0 : 1;
}

uint8_t button_was_released(
  gamepad_button_id button,
  struct gamepad_input const *const gamepad
) {
  return
    (button_is_up(button, gamepad) &&
    (gamepad->previous_buttons >> button) & 1) ?
    1 : 0;
}