#ifndef _LIGHT_COMPONENT_H_
#define _LIGHT_COMPONENT_H_ 1

#include <glm/vec3.hpp>

enum LightAttenuationType{
  Constant = 0,
  Linear = 1,
  Quadratic = 2
};

struct LightComponent{

  glm::vec3 color;
  float attenuation;
  LightAttenuationType attenuation_type;

};

#endif
