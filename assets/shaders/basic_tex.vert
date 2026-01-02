#version 430 core
layout (location = 0) in vec3 _pos;
layout (location = 1) in vec3 _norm;
layout (location = 2) in vec3 _tangent;
layout (location = 3) in vec3 _bittangent;
layout (location = 4) in vec2 _tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 model_transform;

uniform vec3 camera_pos;
uniform vec3 camera_front;

out vec3 pos;
out vec2 tex;
out mat3 TBN;

void main()
{
  gl_Position = projection * view * model * vec4(_pos.x, _pos.y, _pos.z, 1.0);
  pos = (model * vec4(_pos, 1.0f)).xyz;
  tex = _tex;

  vec3 T = normalize(vec3(model * vec4(_tangent,   0.0)));
  vec3 B = normalize(vec3(model * vec4(_bittangent, 0.0)));
  vec3 N = normalize(vec3(model * vec4(_norm,    0.0)));
  TBN = mat3(T, B, N);

}
