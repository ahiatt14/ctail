#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tail.h"
#include "normals.h"
#include "parser.h"

int main(int argc, char *argv[]) {

  FILE *obj_file = NULL;

  if (argc != 3) {
    printf("must include filepath and shading type\n");
    return 1;
  }

  if (
    strcmp(argv[2], "smooth") != 0 &&
    strcmp(argv[2], "flat") != 0
  ) {
    printf(
      "2nd arg must be \"smooth\" or \"flat\"\n"
      "\"%s\" was provided\n", argv[2]
    );
    return 1;
  }

  obj_file = fopen(argv[1], "r");
  if (obj_file == NULL) {
    printf("obj file not found.\n");
    return 1;
  }
  
  if (strcmp(argv[2], "flat") == 0) {
    parse_obj_into_flat_mesh(obj_file);
    print_flat_mesh();
  }
  if (strcmp(argv[2], "smooth") == 0) {
    parse_obj_into_smooth_mesh(obj_file);
    print_smooth_mesh();
  }

  fclose(obj_file);
  return 0;
}