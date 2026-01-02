#ifndef _SHADER_COMPONENT_H_
#define _SHADER_COMPONENT_H_ 1

#include "opengl_headers.h"
#include <string>

struct ShaderComponent{
  std::string key;
};

struct ShaderComponentData{
  GLuint program;
};

#endif
