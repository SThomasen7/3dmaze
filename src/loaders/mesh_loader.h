#ifndef _MESH_LOADER_H_
#define _MESH_LOADER_H_ 1

#include "mesh_component.h"
#include <string>

class MeshLoader{

public:

  static MeshComponent load(std::string filename);
  static void free(MeshComponent& mesh);

private:
  MeshLoader() { }

};


#endif 
