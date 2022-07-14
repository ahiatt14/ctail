#include <stdint.h>

#include "gamepad.h"

// TODO: test us!
// TODO: hide the button code type with typedef?
uint8_t button_is_down(uint16_t button, uint16_t state) {
  return (state >> button) & 1;
}

uint8_t button_is_up(uint16_t button, uint16_t state) {
  return button_is_down(button, state) ? 0 : 1;
}

uint8_t button_was_released(
  uint16_t button,
  uint16_t state,
  uint16_t previous_state
) {
  return
    !button_is_down(button, state) &&
    button_is_down(button, previous_state) ?
    1 : 0;
}