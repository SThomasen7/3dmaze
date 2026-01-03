#ifndef _SHADER_LOADER_H_
#define _SHADER_LOADER_H_ 1

#include "shader_component.h"
#include <string>
#include <memory>

class ShaderLoader{

public:
  static std::shared_ptr<ShaderComponentData> load(
      std::string vertex_name, std::string fragment_name
  );

private:
  ShaderLoader() {};

};


#endif
