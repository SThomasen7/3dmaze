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

void main()
{
  gl_Position = projection * view * model * vec4(_pos.x, _pos.y, _pos.z, 1.0);
  pos = (model * vec4(_pos.x, _pos.y, _pos.z, 1.0)).xyz;
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  norm = normalize(normalMatrix * _norm);
  tangent = _tangent;
  bittangent = _bittangent;
  tex = _tex;
  //color = vec3(0.0f, (_pos.y+1)/2, 0.0);
  //color = normalize(_norm) * 0.5 + 0.5;
}
