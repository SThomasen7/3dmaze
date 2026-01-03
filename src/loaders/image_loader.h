#ifndef _IMAGE_LOADER_H_
#define _IMAGE_LOADER_H_ 1

#define STB_IMAGE_IMPLEMENTATION

#include "image.h"

class ImageLoader{

public:

  static Image load(std::string filename);
  static void free(Image& image);

private:
  ImageLoader() {}

};

#endif
