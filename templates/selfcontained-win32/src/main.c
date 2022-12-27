#include "tail.h"

#include "cube_mesh.h"
#include "fur_texture.h"
#include "cube_frag.h"
#include "cube_vert.h"

const struct vec3 ORIGIN = {0, 0, 0};
const struct coordinate_space WORLDSPACE = {
  { 0, 1, 0 },
  { 1, 0, 0 },
  { 0, 0, -1 }
};

const struct vec3 COLOR_WHITE = { 1, 1, 1 };
const struct vec3 COLOR_AQUA_BLUE = { 0.4f, 0.56f, 0.63f };

struct window_api window;
struct gpu_api gpu;
struct gamepad_input gamepad;

struct transform cube_transform = { .scale = 1 };
struct m4x4 cube_local_to_world;
struct shader cube_shader;

struct viewport vwprt;
struct camera cam;

int main() {

  if (!window__create_fullscreen_game(
    "Self-Contained Win32 Game!",
    REQUEST_VSYNC_ON,
    MSAA_SAMPLES_8,
    &window
  )) return 1;

  gpu__create_api(&gpu);
  gpu.cull_back_faces();

  viewport__set_width(gpu.get_viewport_width(), &vwprt);
  viewport__set_height(gpu.get_viewport_height(), &vwprt);

  cam.position = (struct vec3){ 0, 0, -5 };
  cam.look_target = ORIGIN;
  cam.horizontal_fov_in_deg = 80;
  cam.near_clip_distance = 0.1f;
  cam.far_clip_distance = 6;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(&vwprt, &cam);

  gpu.copy_static_mesh_to_gpu(&cube_mesh);
  gpu.copy_texture_to_gpu(&fur_texture);
  cube_shader.frag_src = cube_frag_src;
  cube_shader.vert_src = cube_vert_src;
  gpu.copy_shader_to_gpu(&cube_shader);

  while (!window.received_closed_event()) {
    window.poll_events();

    // UPDATE
    window.get_gamepad_input(&gamepad);

    cube_transform.rotation_in_deg.y += 0.6f;
    cube_transform.rotation_in_deg.x += 0.4f;
    space__create_model(
      &WORLDSPACE,
      &cube_transform,
      &cube_local_to_world
    );

    // DRAW
    gpu.clear(&COLOR_WHITE);

    gpu.select_shader(&cube_shader);
    gpu.select_texture(&fur_texture);
    gpu.set_shader_m4x4(&cube_shader, "model", &cube_local_to_world);
    gpu.set_shader_m4x4(&cube_shader, "view", &cam.lookat);
    gpu.set_shader_m4x4(&cube_shader, "projection", &cam.projection);

    gpu.draw_mesh(&cube_mesh);
    
    window.request_buffer_swap();
  }
  
  window.end();
  return 0;
}