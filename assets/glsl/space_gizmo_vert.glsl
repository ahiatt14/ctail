#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 projection = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

uniform vec3 up;
uniform vec3 right;
uniform vec3 forward;

out VS_OUT {
  vec4 up;
  vec4 right;
  vec4 forward;
} vs_out;

void main()
{
  mat3 axesVM = mat3(transpose(inverse(view * model)));
  vs_out.up = projection * vec4(normalize(axesVM * up), 0.0);
  vs_out.right = projection * vec4(normalize(axesVM * right), 0.0);
  vs_out.forward = projection * vec4(normalize(axesVM * forward), 0.0);

  gl_Position = projection * view * model * vec4(position, 1.0);
}