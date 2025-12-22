#ifndef _MESH_COMPONENT_H_
#define _MESH_COMPONENT_H_ 1

struct Mesh{
  float* vertex;
  float* normal;
  float* tangent; 
  float* bittangent;
  float* tex;
  unsigned int* index;

  size_t vertex_count;
  size_t index_count;

  bool has_normals; // If we have normals, we have tan and bitan
  bool has_uv;
};


struct MeshComponent{

  Mesh* meshes;
  size_t mesh_count;

}


#endif
