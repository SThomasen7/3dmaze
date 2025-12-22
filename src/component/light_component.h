#ifndef _LIGHT_COMPONENT_H_
#define _LIGHT_COMPONENT_H_ 1

#include <glm/vec3.hpp>

enum LightAttenuationType{
  Constant,
  Linear,
  Quadratic
};

struct LightComponent{

  glm::vec3 color;
  float attenuation;
  LightAttenuationType attenuation_type;

};

#endif
