#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

void str_to_upper(
  char* str
);

int main(int argc, char *argv[]) {

  if (argc != 4) {
    printf("Sourcify-png requires 3 argument.\n");
    return 1;
  }

  char *end_char = NULL;
  long int desired_channels = strtol(argv[2], &end_char, 10);

  if (desired_channels == 0 || desired_channels > 4) {
    printf(
      "Second argument to sourcify-png must be 1 thru 4 for\n"
      "number of channels\n"
    );
    return 1;
  }

  int width = 0;
  int height = 0;
  int channel_count = 0;

  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load(
    argv[1],
    &width,
    &height,
    &channel_count,
    desired_channels
  );

  if (data == NULL) {
    printf("Sourcify-png could not load %s\n", argv[1]);
    return 1;
  }

  char png_filename[200] = {0};
  filename_from_path(png_filename, argv[1], 200);

  char base_output_filepath[200] = {0};
  strcat(base_output_filepath, argv[3]);
  strcat(base_output_filepath, png_filename);

  char src_output_filepath[200] = {0};
  strcpy(src_output_filepath, base_output_filepath);
  strcat(src_output_filepath, "_texture.c");

  char header_output_filepath[200] = {0};
  strcpy(header_output_filepath, base_output_filepath);
  strcat(header_output_filepath, "_texture.h");

  char src_var_name[200];
  strcpy(src_var_name, png_filename);
  str_to_upper(src_var_name);
  strcat(src_var_name, "_TAIL_TEXTURE");

  // HEADER FILE
  FILE *header_file = fopen(header_output_filepath, "w");
  if (header_file == NULL) {
    printf(
      "Sourcify-png could not open header file %s for writing.\n",
      header_output_filepath
    );
    return 1;
  }
  fprintf(header_file, "#ifndef __TAIL_%s__\n", src_var_name);
  fprintf(header_file, "#define __TAIL_%s__\n", src_var_name);
  fprintf(header_file, "#include \"tail.h\"\n");
  fprintf(
    header_file,
    "extern Texture %s;\n",
    src_var_name
  );
  fprintf(header_file, "#endif\n");

  // SRC FILE
  int buffer_length = width * height * desired_channels;
  FILE *src_file = fopen(src_output_filepath, "w");
  if (src_file == NULL) {
    printf(
      "Sourcify-png could not open src file %s for writing.\n",
      src_output_filepath,
      png_filename
    );
    return 1;
  }
  fprintf(src_file, "#include \"tail.h\"\n");
  fprintf(src_file, "#include \"%s_texture.h\"\n", png_filename);
  fprintf(src_file, "Texture %s = {\n", src_var_name);
  fprintf(src_file, ".width = %i,\n", width);
  fprintf(src_file, ".height = %i,\n", height);
  fprintf(src_file, ".channel_count = %i,\n", desired_channels);
  fprintf(src_file, ".name = \"%s\",\n", png_filename);
  fprintf(src_file, ".data = (unsigned char[]){\n");
  for (int i = 0; i < buffer_length; i++) {
    fprintf(src_file, "%i,", data[i]);
    if (i % 16 == 0) fprintf(src_file, "\n");
  }
  fprintf(src_file, "}};");

  fclose(header_file);
  fclose(src_file);
  stbi_image_free(data);
  return 0;
}

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
) {
  char *last_slash = strrchr(filepath, '/');
  if (last_slash == NULL) last_slash = filepath - 1;
  char *last_dot = strrchr(filepath, '.');
  strncpy(filename_out, last_slash, last_dot - ++last_slash);
}

void str_to_upper(
  char* str
) {
  int i = 0;
  while (str[i]) {
    str[i] = toupper(str[i]);
    i++;
  }
}