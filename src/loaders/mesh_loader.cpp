#include "mesh_loader.h"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/postprocess.h>
#include <assimp/scene.h> // Output data structure
#include <stdexcept>
                          
#include "consts.h"
#include "logger.h"

void load_triangle_meshes(MeshComponent& mesh, std::string filename);
Mesh load_triangle_mesh(aiMesh* mesh);

MeshComponent MeshLoader::load(std::string filename){
  MeshComponent mesh;
  LOG(LL::Verbose, "Loading mesh: ");
  LOG(LL::Verbose, asset_path+std::string("obj/")+filename);
  load_triangle_meshes(mesh, asset_path+std::string("obj/")+filename);
  return mesh;
}

void MeshLoader::free(MeshComponent& mesh_component){
  // Free the buffer data
  for(size_t i = 0; i < mesh_component.mesh_count; i++){
    Mesh& mesh = mesh_component.meshes[i];
  
    if(mesh.index != nullptr){
      delete[] mesh.index;
      mesh.index = nullptr;
      mesh.index_count = 0;
    }
    if(mesh.vertex != nullptr){
      delete[] mesh.vertex;
      mesh.vertex = nullptr;
      mesh.vertex_count = 0;
    }
    if(mesh.normal != nullptr){
      delete[] mesh.normal;
      mesh.normal = nullptr;
    }
    if(mesh.tex != nullptr){
      delete[] mesh.tex;
      mesh.tex = nullptr;
    }
    if(mesh.tangent != nullptr){
      delete[] mesh.tangent;
      mesh.tangent = nullptr;
    }
    if(mesh.bittangent != nullptr){
      delete[] mesh.bittangent;
      mesh.bittangent = nullptr;
    }
  }
  delete[] mesh_component.meshes;
  mesh_component.mesh_count = 0;
}

void load_triangle_meshes(MeshComponent& mesh, std::string filename){
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename,
                    aiProcess_Triangulate | 
                    aiProcess_GenSmoothNormals |
                    aiProcess_FlipUVs | 
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_GenUVCoords | 
                    aiProcess_CalcTangentSpace
  );

  // Check if the file name can be opened.
  if (scene == nullptr) {
    LOG(LL::Error, "triangle meshes could not be loaded from: ", filename);
    throw std::runtime_error(std::string("Error reading mesh file: ") + filename);
  }

  aiMesh **meshes = scene->mMeshes;
  size_t num_meshes = scene->mNumMeshes;

  mesh.mesh_count = num_meshes;
  mesh.meshes = new Mesh[num_meshes];

  // Load triangle meshes
  for (size_t i = 0; i < scene->mNumMeshes; i++) {
    mesh.meshes[i] = load_triangle_mesh(meshes[i]);
  }
}

Mesh load_triangle_mesh(aiMesh* mesh){
  Mesh out_mesh;
  out_mesh.has_normals = mesh->HasNormals();
  out_mesh.has_uv = mesh->HasTextureCoords(0);

  const size_t floats_per_vert = 3;
  out_mesh.vertex = new float[mesh->mNumVertices * floats_per_vert];
  out_mesh.vertex_count = mesh->mNumVertices;

  out_mesh.index = new unsigned int[mesh->mNumFaces * 3];
  out_mesh.index_count = mesh->mNumFaces * 3;

  if(out_mesh.has_normals) {
    out_mesh.normal = new float[mesh->mNumVertices * 3];
    out_mesh.tangent = new float[mesh->mNumVertices * 3];
    out_mesh.bittangent = new float[mesh->mNumVertices * 3];
  }
  if(out_mesh.has_uv) {
    out_mesh.tex = new float[mesh->mNumVertices * 2];
  }

  // Copy vertex data into vertex buffer
  for(size_t vert_idx = 0; vert_idx < mesh->mNumVertices; vert_idx++){
    aiVector3D &vec = mesh->mVertices[vert_idx];
    out_mesh.vertex[(vert_idx * floats_per_vert) + 0] = vec.x;
    out_mesh.vertex[(vert_idx * floats_per_vert) + 1] = vec.y;
    out_mesh.vertex[(vert_idx * floats_per_vert) + 2] = vec.z;

    // Copy the normal data
    if(out_mesh.has_normals){
      // Set normals
      aiVector3D &vec = mesh->mNormals[vert_idx];
      out_mesh.normal[(vert_idx * 3) + 0] = vec.x;
      out_mesh.normal[(vert_idx * 3) + 1] = vec.y;
      out_mesh.normal[(vert_idx * 3) + 2] = vec.z;

      // Set tangent vectors
      aiVector3D& tangent = mesh->mTangents[vert_idx];
      out_mesh.tangent[(vert_idx * 3) + 0] = tangent.x;
      out_mesh.tangent[(vert_idx * 3) + 1] = tangent.y;
      out_mesh.tangent[(vert_idx * 3) + 2] = tangent.z;

      // Set bittangent vectors
      aiVector3D& bittangent = mesh->mBitangents[vert_idx];
      out_mesh.bittangent[(vert_idx * 3) + 0] = bittangent.x;
      out_mesh.bittangent[(vert_idx * 3) + 1] = bittangent.y;
      out_mesh.bittangent[(vert_idx * 3) + 2] = bittangent.z;
    }
    
    // Copy the texture coord data
    if(out_mesh.has_uv){
      aiVector3D &vec = mesh->mTextureCoords[0][vert_idx];
      out_mesh.tex[(vert_idx * 2) + 0] = vec.x;
      out_mesh.tex[(vert_idx * 2) + 1] = vec.y;
    }
  }

  // Load the index data
  for (size_t face_idx = 0; face_idx < mesh->mNumFaces; face_idx++){
    aiFace face = mesh->mFaces[face_idx];
    out_mesh.index[(face_idx * 3) + 0] = (unsigned int)face.mIndices[0];
    out_mesh.index[(face_idx * 3) + 1] = (unsigned int)face.mIndices[1];
    out_mesh.index[(face_idx * 3) + 2] = (unsigned int)face.mIndices[2];
  }

  return out_mesh;
}
