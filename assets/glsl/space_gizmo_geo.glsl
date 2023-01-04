#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

out vec3 color;

in VS_OUT {
  vec4 up;
  vec4 right;
  vec4 forward;
} gs_in[];

void generate_line(vec4 axis, vec3 _color) {

  color = _color;

  gl_Position = gl_in[0].gl_Position - axis * 0.2; 
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + axis;
  EmitVertex();

  EndPrimitive();
}

void main() {
  generate_line(gs_in[0].up, vec3(0, 1, 0));
  generate_line(gs_in[0].right, vec3(1, 0, 0));
  generate_line(gs_in[0].forward, vec3(0, 0, 1));
}