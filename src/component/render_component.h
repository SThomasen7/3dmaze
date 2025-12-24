#ifndef _RENDER_COMPONENT_H_
#define _RENDER_COMPONENT_H_ 1

#include "opengl_headers.h"

struct RenderComponent{
  size_t mesh_count;
  GLuint* VAO;
  GLuint* VBO;
  GLuint* VIO;
  size_t* index_count;
};

#endif
