#ifndef _MESH_LOADER_H_
#define _MESH_LOADER_H_ 1

#include "mesh_component.h"
#include <string>
#include <memory>

class MeshLoader{

public:

  static std::shared_ptr<MeshComponentData> load(std::string filename);
  static void free(std::shared_ptr<MeshComponentData> mesh);

private:
  MeshLoader() { }

};


#endif 
