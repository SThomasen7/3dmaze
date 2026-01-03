#ifndef _TEXTURE_COMPONENT_H_
#define _TEXTURE_COMPONENT_H_ 1

#include <string>
#include "image.h"
#include "opengl_headers.h"

struct TextureComponent{
  std::string key;
};

struct TextureComponentData{
  Image image;
  GLuint id;
};

#endif
