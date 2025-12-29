#include "render_system.h"
#include "logger.h"
#include "entity_manager.h"
#include "opengl_headers.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


RenderSystem::RenderSystem(){ }

RenderComponent create_render_component(const MeshComponent& mesh);
void destroy_render_component(RenderComponent& render);

void RenderSystem::init(){
  LOG(LL::Info, "Initializing Render System.");
}

void RenderSystem::process(Scene& scene, float dt){

  EntityManager& entity_manager = scene.getEntityManager();

  // TODO add error handling and logging.
  if(scene.active_camera == -1){
    LOG(LL::Error, "No active camera found! Skipping rendering...");
    return;
  }
  
  int camera_idx = 0;
  using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<CameraComponent>();

  CameraComponent camera_component;
  PositionComponent camera_pos_component;
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    if(camera_idx == scene.active_camera){
      camera_component = entity_manager.getCamera(*entity_ptr);
      camera_pos_component = entity_manager.getPosition(*entity_ptr);
      break;
    }
  }
  entity_manager.destroyEntityView(&eview);

  // Calculate the camera transformation
  glm::mat4 vp(1.0f);
  glm::mat4 projection(1.0f);
  glm::mat4 view(1.0f);

  if(camera_component.projection_type == CameraProjection::Perspective){
    projection = glm::perspective(
        glm::radians(camera_component.fov), 
        camera_component.buffer_x/camera_component.buffer_y,
        camera_component.z_near, camera_component.z_far
    );
  }
  else{
    projection = glm::ortho(-camera_component.buffer_x/200.0f, camera_component.buffer_x/200.0f,
                            -camera_component.buffer_y/200.0f, camera_component.buffer_y/200.0f,
                            camera_component.z_near, camera_component.z_far);
  }

  // Get the view and model matrix
	view = glm::lookAt(camera_pos_component.position, 
      camera_pos_component.position+camera_component.lookat, 
      camera_component.up);

  // Calculate and return the transformation
	vp = projection * view;

  // set the base color
  glClearColor(0.10f, 0.15f, 0.30f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Create the view
  eview = 
    entity_manager.createEntityView<RenderComponent, ShaderComponent, TransformComponent>();

  //LOG(LL::Verbose, "Starting render..");
  // Get the entities with mesh components and get the mesh data
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    //LOG(LL::Verbose, "Rendering mesh!");
    RenderComponent render_c = entity_manager.getRender(*entity_ptr);
    ShaderComponent shader_c = entity_manager.getShader(*entity_ptr);
    TransformComponent transform_c = entity_manager.getTransform(*entity_ptr);

    // set the shader
    GLuint program = shader_c.program;
    glUseProgram(program);
    CHECK_OGL_ERROR();

    // get the uniform from the shader
    GLint location = glGetUniformLocation(program, "mvp");
    CHECK_OGL_ERROR();

    // calculate the transform and set
    glm::mat4 asset_mvp;
    asset_mvp = vp * transform_c.transformation;

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(asset_mvp));
    CHECK_OGL_ERROR();
    

    for(size_t i = 0; i < render_c.mesh_count; i++){
      glBindVertexArray(render_c.VAO[i]);
      CHECK_OGL_ERROR();
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_c.VIO[i]);
      CHECK_OGL_ERROR();
      glBindBuffer(GL_ARRAY_BUFFER,render_c.VBO[i]);
      CHECK_OGL_ERROR();

      glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(render_c.index_count[i]), 
          GL_UNSIGNED_INT, 0);
      CHECK_OGL_ERROR();
      glBindVertexArray(0);
    }
  }
  entity_manager.destroyEntityView(&eview);
}

void RenderSystem::shutdown(){
  LOG(LL::Info, "Shutting down Render System.");
}


void RenderSystem::preLoadScene(Scene& scene){
  
  // TODO find a better place for these
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  EntityManager& entity_manager = scene.getEntityManager();

  // Create the view
  using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<MeshComponent>();

  // Get the entities with mesh components and get the mesh data
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    MeshComponent mesh = entity_manager.getMesh(*entity_ptr);
    RenderComponent render = create_render_component(mesh);
    entity_manager.addRenderComponent(*entity_ptr, render);
  }
  entity_manager.destroyEntityView(&eview);

}

void RenderSystem::clearScene(Scene& scene){

  EntityManager& entity_manager = scene.getEntityManager();

  // Create the view
  using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<RenderComponent>();

  // Get the entities with mesh components and get the mesh data
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    RenderComponent component = entity_manager.getRender(*entity_ptr);
    destroy_render_component(component);
  }
}

RenderComponent create_render_component(const MeshComponent& meshes){

  RenderComponent obuffs;
  obuffs.mesh_count = meshes.mesh_count;
  obuffs.VAO = new GLuint[obuffs.mesh_count];
  obuffs.VBO = new GLuint[obuffs.mesh_count];
  obuffs.VIO = new GLuint[obuffs.mesh_count];
  obuffs.index_count = new size_t[obuffs.mesh_count];

  glGenVertexArrays(obuffs.mesh_count, obuffs.VAO);
  CHECK_OGL_ERROR();
  glGenBuffers(obuffs.mesh_count, obuffs.VBO);
  CHECK_OGL_ERROR();
  glGenBuffers(obuffs.mesh_count, obuffs.VIO);
  CHECK_OGL_ERROR();

  for(size_t index = 0; index < meshes.mesh_count; index++){
    Mesh mesh = meshes.meshes[index];

    size_t vertex_size = 3; // 3 for position
    if(mesh.has_normals){
      vertex_size += 9; // normal + tan + bitan
    }
    if(mesh.has_uv){
      vertex_size += 2; // u v coords
    }
    
    // Bind the vertex attribute object
    glBindVertexArray(obuffs.VAO[index]);
    CHECK_OGL_ERROR();

    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, obuffs.VBO[index]);

    // allocate buffer memory
    glBufferData(GL_ARRAY_BUFFER,
               sizeof(float) * mesh.vertex_count * vertex_size, NULL,
               GL_STATIC_DRAW);
    CHECK_OGL_ERROR();

    // copy position data to the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(float) * mesh.vertex_count * 3,
                  mesh.vertex);
    CHECK_OGL_ERROR();
    size_t offset = sizeof(float) * mesh.vertex_count * 3;
    if(mesh.has_normals){

      // copy normal data
      glBufferSubData(GL_ARRAY_BUFFER, offset,
                    sizeof(float) * mesh.vertex_count * 3,
                    mesh.normal);
      offset += sizeof(float) * mesh.vertex_count * 3;
      CHECK_OGL_ERROR();

      // copy tan data
      glBufferSubData(GL_ARRAY_BUFFER, offset,
                    sizeof(float) * mesh.vertex_count * 3,
                    mesh.tangent);
      offset += sizeof(float) * mesh.vertex_count * 3;
      CHECK_OGL_ERROR();
      
      // copy bittan data
      glBufferSubData(GL_ARRAY_BUFFER, offset,
                    sizeof(float) * mesh.vertex_count * 3,
                    mesh.bittangent);
      offset += sizeof(float) * mesh.vertex_count * 3;
      CHECK_OGL_ERROR();
    }

    if(mesh.has_uv){
      // copy textre data
      glBufferSubData(GL_ARRAY_BUFFER, offset,
                    sizeof(float) * mesh.vertex_count * 2,
                    mesh.tex);
      offset += sizeof(float) * mesh.vertex_count * 2;
      CHECK_OGL_ERROR();
    }

    // Element array data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obuffs.VIO[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * mesh.index_count, mesh.index,
                 GL_STATIC_DRAW);
    CHECK_OGL_ERROR();
    obuffs.index_count[index] = mesh.index_count;

    // Configure vertex attribute data
    // Vertex position data always is in position 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                          (void *)0);
    CHECK_OGL_ERROR();
    glEnableVertexAttribArray(0);
    CHECK_OGL_ERROR();

    offset = 3;

    if(mesh.has_normals){
      // Normals is 1
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                            (void *)(offset * sizeof(float) * mesh.vertex_count));
      CHECK_OGL_ERROR();
      glEnableVertexAttribArray(1);
      CHECK_OGL_ERROR();
      offset += 3;

      // tangent 2
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0,
                            (void *)(offset * sizeof(float) * mesh.vertex_count));
      CHECK_OGL_ERROR();
      glEnableVertexAttribArray(2);
      CHECK_OGL_ERROR();
      offset += 3;

      // tangent 3
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0,
                            (void *)(offset * sizeof(float) * mesh.vertex_count));
      CHECK_OGL_ERROR();
      glEnableVertexAttribArray(3);
      CHECK_OGL_ERROR();
      offset += 3;
    }

    if(mesh.has_uv){
      // tex coords 4
      glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0,
                            (void *)(offset * sizeof(float) * mesh.vertex_count));
      CHECK_OGL_ERROR();
      glEnableVertexAttribArray(4);
      CHECK_OGL_ERROR();
      offset += 2;
    }

    // unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  }
  return obuffs;
}

void destroy_render_component(RenderComponent& render){

  for(size_t i = 0; i < render.mesh_count; i++){

    glDeleteBuffers(render.mesh_count, render.VAO);
    CHECK_OGL_ERROR();
    glDeleteBuffers(render.mesh_count, render.VBO);
    CHECK_OGL_ERROR();
    glDeleteBuffers(render.mesh_count, render.VIO);
    CHECK_OGL_ERROR();

    delete[] render.VAO;
    delete[] render.VBO;
    delete[] render.VIO;
    delete[] render.index_count;

  }
}


