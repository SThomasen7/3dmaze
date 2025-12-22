#ifndef _SHADER_LOADER_H_
#define _SHADER_LOADER_H_ 1

#include "shader_component.h"
#include <string>

class ShaderLoader{

public:
  static ShaderComponent loadShaderComponent(
      std::string vertex_name, std::string fragment_name
  );

private:
  virtual ShaderLoader() = 0;

};


#endif
