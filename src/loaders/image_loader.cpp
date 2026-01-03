#include "image_loader.h"
#include "stb_image.h"
#include "logger.h"

std::shared_ptr<Image> ImageLoader::load(std::string filename){
  std::shared_ptr<Image> image = std::make_shared<Image>();
  image->width = 0;
  image->height = 0;
  image->n_channels = 0;
  image->data = nullptr;

  image->data = stbi_load(filename.c_str(), &(image->width), 
      &(image->height), &(image->n_channels), 0);

  if(!image->data){
    LOG(LL::Warn, filename, " could not be loaded.");
    return nullptr;
  }

  return image;
}

void ImageLoader::free(std::shared_ptr<Image> image){
  image->width = 0;
  image->height = 0;
  image->n_channels = 0;
  stbi_image_free(image->data);
  image->data = nullptr;
}

