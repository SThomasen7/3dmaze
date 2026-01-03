#ifndef _TEXTURE_COMPONENT_H_
#define _TEXTURE_COMPONENT_H_ 1

#include <string>
#include <memory>
#include "image.h"
#include "opengl_headers.h"

struct TextureComponent{
  std::string key;
};

struct TextureComponentData{
  std::shared_ptr<Image> color_map;
  std::shared_ptr<Image> normal_map;
  GLuint color_id;
  GLuint normal_id;
};

#endif
