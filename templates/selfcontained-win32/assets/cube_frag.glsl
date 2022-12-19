#version 330 core

uniform sampler2D tex;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  FragColor = texture(tex, fs_in.tex_uv);
}