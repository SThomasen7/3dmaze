#ifndef _MESH_LOADER_H_
#define _MESH_LOADER_H_ 1

#include "mesh_component.h"
#include <string>

class MeshLoader{

public:

  static MeshComponentData load(std::string filename);
  static void free(MeshComponentData& mesh);

private:
  MeshLoader() { }

};


#endif 
