#version 420 core
layout (location = 0) in vec3 _pos;
layout (location = 1) in vec3 _norm;
layout (location = 2) in vec3 _tangent;
layout (location = 3) in vec3 _bittangent;
layout (location = 4) in vec2 _tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec3 tangent;
out vec3 bittangent;
out vec2 tex;
out mat3 TBN;

void main()
{
  gl_Position = projection * view * model * vec4(_pos.x, _pos.y, _pos.z, 1.0);
  pos = (model * vec4(_pos.x, _pos.y, _pos.z, 1.0)).xyz;
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  norm = normalize(normalMatrix * _norm);
  tangent = _tangent;
  bittangent = _bittangent;
  tex = _tex;

  vec3 N = normalize(normalMatrix * _norm);
  vec3 T = normalize(normalMatrix * _tangent);
  vec3 B = normalize(normalMatrix * _bittangent);

  TBN = mat3(T, B, N);
}
