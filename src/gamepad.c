#include <stdint.h>

#include "gamepad.h"

// TODO: test us!
uint8_t is_down(uint16_t button, uint16_t state) {
  return (state >> button) & 1;
}

uint8_t is_up(uint16_t button, uint16_t state) {
  return is_down(button, state) ? 0 : 1;
}

uint8_t was_released(
  uint16_t button,
  uint16_t state,
  uint16_t previous_state
) {
  return
    !is_down(button, state) &&
    is_down(button, previous_state) ?
    1 : 0;
}