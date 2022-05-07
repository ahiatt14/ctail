#include <stdio.h> // TODO: could inject these functions
#include <stdlib.h>
#include <string.h>

#include "debugger.h"

#define MAX_ITEMS 30
#define MAX_LABEL_SIZE 20

// TODO: turn debugger into injectable

static char labels[MAX_ITEMS][MAX_LABEL_SIZE];

static float float_values[MAX_ITEMS];
static float vec3_values[MAX_ITEMS][3];

// 0 for floats, 1 for vec3
static unsigned short int value_types[MAX_ITEMS];

static unsigned short int count;

static int char_count_in_longest_label() {
  int char_count = 0;
  for (int i = 0; i < count; i++) {
    int length = strlen(labels[i]);
    if (length > char_count) char_count = length;
  }
  return char_count;
}

void debugger__clear_and_print() {

  system("clear");
  printf("]------------------------------------------------------------[\n");

  int longest_label_length = char_count_in_longest_label();
  for (int i = 0; i < count; i++) {

    // LABEL AND PADDING
    printf("| %s", labels[i]);
    for (int j = 0; j < longest_label_length - strlen(labels[i]); j++)
      printf("%c", ' ');
    printf("  :  ");

    // VALUE
    if (value_types[i]) {
      printf(
        "x %.5f y %.5f z %.5f",
        vec3_values[i][0], vec3_values[i][1], vec3_values[i][2]
      );
    } else {
      printf("%.5f", float_values[i]);
    }
    printf("\n");
  }
  
  printf("]------------------------------------------------------------[\n");
  count = 0;
}

void debugger__add_float(const char *label, float value) {
  if (count + 1 > MAX_ITEMS) return;
  float_values[count] = value;
  value_types[count] = 0;
  strncpy(labels[count++], label, sizeof(char) * MAX_LABEL_SIZE);
}

void debugger__add_vec3(const char *label, float *values) {
  if (count + 1 > MAX_ITEMS) return;
  memcpy(vec3_values[count], values, sizeof(float) * 3);
  value_types[count] = 1;
  strncpy(labels[count++], label, sizeof(char) * MAX_LABEL_SIZE);
}