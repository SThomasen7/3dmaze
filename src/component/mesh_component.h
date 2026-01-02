#ifndef _MESH_COMPONENT_H_
#define _MESH_COMPONENT_H_ 1

#include <cstddef>
#include <string>

struct Mesh{
  float* vertex;
  float* normal;
  float* tangent; 
  float* bittangent;
  float* tex;
  unsigned int* index;

  size_t vertex_count;
  size_t index_count;

  bool has_normals;
  bool has_uv; // If we have texture coords, 
               // then we have tan and bitan
};

struct MeshComponent{
  std::string key;
};

struct MeshComponentData{
  Mesh* meshes;
  size_t mesh_count;
};

#endif
