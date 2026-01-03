#include "image_loader.h"
#include "stb_image.h"
#include "logger.h"

Image ImageLoader::load(std::string filename){
  Image image;
  image.width = 0;
  image.height = 0;
  image.n_channels = 0;
  image.data = nullptr;

  image.data = stbi_load(filename.c_str(), &(image.width), 
      &(image.height), &(image.n_channels), 0);

  if(!image.data){
    LOG(LL::Warn, filename, " could not be loaded.");
  }

  return image;
}

void ImageLoader::free(Image& image){
  image.width = 0;
  image.height = 0;
  image.n_channels = 0;
  stbi_image_free(image.data);
  image.data = nullptr;
}

