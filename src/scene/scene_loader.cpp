#include "scene_loader.h"
#include "logger.h"
#include "transform_component.h"
#include "mesh_loader.h"
#include "shader_loader.h"
#include "consts.h"

#include "transform_component.h"
#include "camera_component.h"
#include "mesh_component.h"
#include "shader_component.h"
#include "light_component.h"
#include "position_component.h"
#include "light_direction_component.h"
#include "light_angle_component.h"

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

const std::string scene_validation_file = base_dir+std::string("/src/misc/scene_validate.xsd");

// Helper function declarations
void create_scene_nodes(EntityManager& entity_manager, xmlNodePtr& xml_node);

void create_camera_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node);
void create_light_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node);
void create_transform_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node);
void create_mesh_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node);
void create_shader_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node);

glm::vec3 load_vec3(xmlNodePtr& node);
glm::vec4 load_vec4(xmlNodePtr& node);
//bool is_named(xmlNodePtr& node, std::string name);
std::string get_name(xmlNodePtr& node);
// End helper function declarations

void SceneLoader::load(Scene& scene, std::string filename){
  xmlSchemaParserCtxtPtr schema_ctxt = xmlSchemaNewParserCtxt(scene_validation_file.c_str());
  if (!schema_ctxt) {
    LOG(LL::Error, "Could not load schema context from ", scene_validation_file);
  }

  xmlSchemaPtr schema = xmlSchemaParse(schema_ctxt);
  xmlSchemaFreeParserCtxt(schema_ctxt);

  if (!schema) {
    LOG(LL::Error, "Could not parse schema context from ", scene_validation_file);
  }

  // Create a validation context
  xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
  if (!valid_ctxt) {
    LOG(LL::Error, "Could not schema validation context from ", scene_validation_file);
    xmlSchemaFree(schema);
  }

  // Parse the XML file
  xmlDocPtr doc = xmlReadFile(filename.c_str(), nullptr, 0);
  if (!doc) {
    LOG(LL::Error, "Could not parse XML file from ", filename);
    xmlSchemaFreeValidCtxt(valid_ctxt);
    xmlSchemaFree(schema);
  }

  // Validate the XML file
  int ret = xmlSchemaValidateDoc(valid_ctxt, doc);
  if(ret > 0){
    LOG(LL::Error, filename, " XML file is invalid according to ", scene_validation_file);
  } else if (ret < 0) {
    LOG(LL::Error, "Validation has generated an internal eror");
  }

  xmlNodePtr root = xmlDocGetRootElement(doc);
  EntityManager& entity_manager = scene.getEntityManager();
  create_scene_nodes(entity_manager, root);

  // The first camera is the active camera
  /*using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<CameraComponent>();*/

  // TODO check that there is a zeroth active camera.
  scene.active_camera = 0;
  /*for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    scene.active_camera = *entity_ptr;
    break;
  }
  entity_manager.destroyEntityView(&eview);*/

  // Cleanup
  xmlFreeDoc(doc);
  xmlSchemaFreeValidCtxt(valid_ctxt);
  xmlSchemaFree(schema);
  xmlCleanupParser();
}


// Recursively load all of the children nodes.
void create_scene_nodes(EntityManager& entity_manager, xmlNodePtr& xml_node){

  bool is_root = false;
  // An entity is wasted on the scene, but that's ok.
  Entity entity = entity_manager.createEntity();
  if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "scene")){
    is_root = true;
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "camera")){
    create_camera_component(entity_manager, entity, xml_node);
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "light")){
    create_light_component(entity_manager, entity, xml_node);
  }

  // Check if any of the child nodes are components
  for (xmlNodePtr cur = xml_node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "transformation")) {
      create_transform_component(entity_manager, entity, cur);
      continue;
    }
    else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "mesh")) {
      create_mesh_component(entity_manager, entity, cur);
      continue;
    }
    else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "shader")) {
      create_shader_component(entity_manager, entity, cur);
      continue;
    }
    create_scene_nodes(entity_manager, cur);
  }
}

void create_camera_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node){
  LOG(LL::Verbose, "Loading camera component.");
  
  float fov;
  float buffer_x, buffer_y;
  float z_near, z_far;
  std::string projection_type;
  glm::vec3 up, lookat, position;

  // get the children nodes
  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "up")) {
      up = glm::normalize(load_vec3(cur));
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "lookat")) {
      lookat = glm::normalize(load_vec3(cur));
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "position")) {
      position = load_vec3(cur);
    }
  }

  xmlChar *fov_cstr = xmlGetProp(node, BAD_CAST "fov");
  xmlChar *buffer_x_cstr = xmlGetProp(node, BAD_CAST "buffer_x");
  xmlChar *buffer_y_cstr = xmlGetProp(node, BAD_CAST "buffer_y");
  xmlChar *z_near_cstr = xmlGetProp(node, BAD_CAST "z_near");
  xmlChar *z_far_cstr = xmlGetProp(node, BAD_CAST "z_far");
  xmlChar *projection_type_cstr = xmlGetProp(node, BAD_CAST "projection_type");

  // Convert xmlChar* to std::string
  std::string fov_str(reinterpret_cast<const char*>(fov_cstr));
  std::string buffer_x_str(reinterpret_cast<const char*>(buffer_x_cstr));
  std::string buffer_y_str(reinterpret_cast<const char*>(buffer_y_cstr));
  std::string z_near_str(reinterpret_cast<const char*>(z_near_cstr));
  std::string z_far_str(reinterpret_cast<const char*>(z_far_cstr));
  std::string projection_type_str(reinterpret_cast<const char*>(projection_type_cstr));

  // Convert string to float
  fov = std::stof(fov_str);
  buffer_x = std::stof(buffer_x_str);
  buffer_y = std::stof(buffer_y_str);
  z_near = std::stof(z_near_str);
  z_far = std::stof(z_far_str);

  CameraProjection type;
  if(projection_type_str == "perspective"){
    type = CameraProjection::Perspective;
  }
  else if(projection_type_str == "orthographic"){
    type = CameraProjection::Orthographic;
  }

  entity_manager.addCameraComponent(entity,
      {
        up, lookat, glm::cross(up, lookat),
        type,
        fov,
        buffer_x,
        buffer_y,
        z_near,
        z_far
      }
  );

  entity_manager.addPositionComponent(entity,
      {
        position
      }
  );

  // Free the attribute values
  xmlFree(fov_cstr);
  xmlFree(buffer_x_cstr);
  xmlFree(buffer_y_cstr);
  xmlFree(z_near_cstr);
  xmlFree(z_far_cstr);
  xmlFree(projection_type_cstr);
}

void create_light_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node){

  LOG(LL::Verbose, "Loading light component.");
  glm::vec3 color, position, direction;
  bool has_direction, has_position = false;
  float attenuation, angle;
  std::string atteuation_type;

  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "color")) {
      color = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "direction")) {
      direction = load_vec3(cur);
      has_direction = true;
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "position")) {
      position = load_vec3(cur);
      has_position = true;
    }
  }

  if(has_direction){
    entity_manager.addLightDirectionComponent(entity,
        {
          direction 
        }
    );
  }

  if(has_position){
    entity_manager.addPositionComponent(entity,
        {
          position 
        }
    );
  }

  xmlChar *att_cstr = xmlGetProp(node, BAD_CAST "attenuation");
  xmlChar *angle_cstr = xmlGetProp(node, BAD_CAST "angle");
  xmlChar *att_t_cstr = xmlGetProp(node, BAD_CAST "attenuation_type");
  std::string att, angle_str, att_t;

  // Convert xmlChar* to std::string
  if(att_cstr != NULL){
    std::string att(reinterpret_cast<const char*>(att_cstr));
    attenuation = std::stof(att);  
  }
  else{
    attenuation = 1.0f;
  }

  if(angle_cstr != NULL){
    std::string angle_str(reinterpret_cast<const char*>(angle_cstr));
    angle = std::stof(angle_str);
    entity_manager.addLightAngleComponent(entity,
        {
          angle
        }
    );
  }

  LightAttenuationType type = LightAttenuationType::Constant;
  if(att_cstr != nullptr){
    std::string att(reinterpret_cast<const char*>(att_t_cstr));
    if(att_t == "constant"){
      type = LightAttenuationType::Constant;
    }
    else if(att_t == "linear"){
      type = LightAttenuationType::Linear;
    }
    else{
      type = LightAttenuationType::Quadratic;
    }
  }

  entity_manager.addLightComponent(entity,
      {
        color,
        attenuation,
        type
      }
  );
}

void create_transform_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node){
  LOG(LL::Verbose, "Loading transform component.");
  // determine if its the 4 vec3 method or not.
  bool has_columns = false;
  std::shared_ptr<TransformComponent> transform_component = 
    std::make_shared<TransformComponent>();

  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "col1")) {
      has_columns = true;
      break;
    }
  }

  // node has 4 columns
  if(has_columns){
    // init the column vectors
    glm::vec4 col1, col2, col3, col4;

    // parse them out of the node
    for(xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
      if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "col1")) {
        col1 = load_vec4(cur);
      }
      else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "col2")) {
        col2 = load_vec4(cur);
      }
      else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "col3")) {
        col3 = load_vec4(cur);
      }
      else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "col4")) {
        col4 = load_vec4(cur);
      }
    }
    
    glm::mat4 transform(col1, col2, col3, col4);
    entity_manager.addTransformComponent(entity, {transform});
  }
  else{

    // init possible transform vectors
    bool has_translate = false;
    bool has_rotate = false;
    bool has_scale = false;
    glm::vec3 translate;
    glm::vec4 rotate;
    glm::vec3 scale;

    // parse them out of the node
    for(xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {

      // check for vec4 type
      if(cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "translate")) {
        translate = load_vec3(cur);
        has_translate = true;
      }
      else if(cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "scale")) {
        scale = load_vec3(cur);
        has_scale = true;
      }
      else if(cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "rotate")) {
        rotate = load_vec4(cur);
        has_rotate = true;
      }

    }

    // set the transformation
    glm::mat4 transform(1.0f);
    if(has_translate){
      transform = glm::translate(transform, translate);
    }
    if(has_rotate){
      transform = glm::rotate(transform, glm::radians(rotate.w),
          glm::normalize(glm::vec3(rotate.x, rotate.y, rotate.z)));
    }
    if(has_scale){
      transform = glm::scale(transform, scale);
    }
    entity_manager.addTransformComponent(entity, {transform});
  }
}

void create_mesh_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node){
  LOG(LL::Verbose, "Loading mesh component.");
  std::string filename = get_name(node); 
  LOG(LL::Verbose, "Mesh file: "+filename);
  entity_manager.addMeshComponent(entity, MeshLoader::load(filename));
}

void create_shader_component(EntityManager& entity_manager, Entity entity, xmlNodePtr& node){
  LOG(LL::Verbose, "Loading shader component.");
  xmlChar *vertex_cstr = xmlGetProp(node, BAD_CAST "vertex");
  xmlChar *fragment_cstr = xmlGetProp(node, BAD_CAST "fragment");

  // Convert xmlChar* to std::string
  std::string vertex_str(reinterpret_cast<const char*>(vertex_cstr));
  std::string fragment_str(reinterpret_cast<const char*>(fragment_cstr));

  xmlFree(fragment_cstr);
  xmlFree(vertex_cstr);

  entity_manager.addShaderComponent(entity, ShaderLoader::load(vertex_str, fragment_str));
}

glm::vec3 load_vec3(xmlNodePtr& node){
  glm::vec3 out;
  xmlChar *x_cstr = xmlGetProp(node, BAD_CAST "x");
  xmlChar *y_cstr = xmlGetProp(node, BAD_CAST "y");
  xmlChar *z_cstr = xmlGetProp(node, BAD_CAST "z");

  // Convert xmlChar* to std::string
  std::string x_str(reinterpret_cast<const char*>(x_cstr));
  std::string y_str(reinterpret_cast<const char*>(y_cstr));
  std::string z_str(reinterpret_cast<const char*>(z_cstr));

  // Convert string to float
  float x = std::stof(x_str);  
  float y = std::stof(y_str);  
  float z = std::stof(z_str);  

  out.x = x;
  out.y = y;
  out.z = z;

  // Free the attribute values
  xmlFree(x_cstr);
  xmlFree(y_cstr);
  xmlFree(z_cstr);

  return out;
}

glm::vec4 load_vec4(xmlNodePtr& node){
  glm::vec4 out;
  xmlChar *x_cstr = xmlGetProp(node, BAD_CAST "x");
  xmlChar *y_cstr = xmlGetProp(node, BAD_CAST "y");
  xmlChar *z_cstr = xmlGetProp(node, BAD_CAST "z");
  xmlChar *w_cstr = xmlGetProp(node, BAD_CAST "w");

  // Convert xmlChar* to std::string
  std::string x_str(reinterpret_cast<const char*>(x_cstr));
  std::string y_str(reinterpret_cast<const char*>(y_cstr));
  std::string z_str(reinterpret_cast<const char*>(z_cstr));
  std::string w_str(reinterpret_cast<const char*>(w_cstr));

  // Convert string to float
  float x = std::stof(x_str);  
  float y = std::stof(y_str);  
  float z = std::stof(z_str);  
  float w = std::stof(w_str);  

  out.x = x;
  out.y = y;
  out.z = z;
  out.w = w;

  // Free the attribute values
  xmlFree(x_cstr);
  xmlFree(y_cstr);
  xmlFree(z_cstr);
  xmlFree(w_cstr);

  return out;
}

/*bool is_named(xmlNodePtr& node, std::string name){
  return get_name(node) == name;
}*/

std::string get_name(xmlNodePtr& node){
  xmlChar *name_cstr = xmlGetProp(node, BAD_CAST "name");
  if(name_cstr == NULL){
    LOG(LL::Warn, "Warning, mesh file name not found");
    return "";
  }
  std::string name_str(reinterpret_cast<const char*>(name_cstr));
  xmlFree(name_cstr);
  return name_str;
}

void SceneLoader::free(Scene& scene, std::string filename){

};
