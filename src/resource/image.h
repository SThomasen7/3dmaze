#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

struct Image{
  int width;
  int height;
  int n_channels;
  unsigned char* data;
};

#endif

