#ifndef _IMAGE_LOADER_H_
#define _IMAGE_LOADER_H_ 1

#define STB_IMAGE_IMPLEMENTATION

#include "image.h"
#include <memory>

class ImageLoader{

public:

  static std::shared_ptr<Image> load(std::string filename);
  static void free(std::shared_ptr<Image> image);

private:
  ImageLoader() {}

};

#endif
