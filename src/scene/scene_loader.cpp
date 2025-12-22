#include "scene_loader.h"
/*#include <fstream>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <vector>
#include <stdexcept>
#include "transform_component.h"
#include "mesh_loader.h"
#include "shader_loader.h"
#include "light_manager.h"
#include "component_manager.h"
#include "camera.h"
#include "asset.h"
#include "logger.h"
#include "consts.h"

#include <iostream>

const std::string scene_validation_file = base_dir+std::string("/src/misc/scene_validate.xsd");

// Helper function declarations
void create_scene_nodes(std::shared_ptr<Scene> scene_node, xmlNodePtr& xml_node);

std::shared_ptr<Camera> create_camera_entity(xmlNodePtr& node);
std::shared_ptr<Light> create_directionallight_entity(xmlNodePtr& node);
std::shared_ptr<Light> create_spotlight_entity(xmlNodePtr& node);
std::shared_ptr<Light> create_pointlight_entity(xmlNodePtr& node);
std::shared_ptr<Asset> create_asset_entity(xmlNodePtr& node);
std::shared_ptr<TransformComponent> create_transform(xmlNodePtr& node);
std::shared_ptr<MeshComponent> create_mesh(xmlNodePtr& node);
std::shared_ptr<ShaderComponent> create_shader(xmlNodePtr& node);

glm::vec3 load_vec3(xmlNodePtr& node);
glm::vec4 load_vec4(xmlNodePtr& node);
bool is_named(xmlNodePtr& node, std::string name);
std::string get_name(xmlNodePtr& node);
// End helper function declarations

std::shared_ptr<Scene> SceneLoader::loadScene(std::string filename){
 // Load the schema
  xmlSchemaParserCtxtPtr schema_ctxt = xmlSchemaNewParserCtxt(scene_validation_file.c_str());
  if (!schema_ctxt) {
    LOG(LL::Error, "Could not load schema context from ", scene_validation_file);
    return nullptr;
  }

  xmlSchemaPtr schema = xmlSchemaParse(schema_ctxt);
  xmlSchemaFreeParserCtxt(schema_ctxt);

  if (!schema) {
    LOG(LL::Error, "Could not parse schema context from ", scene_validation_file);
    return nullptr;
  }

  // Create a validation context
  xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
  if (!valid_ctxt) {
    LOG(LL::Error, "Could not schema validation context from ", scene_validation_file);
    xmlSchemaFree(schema);
    return nullptr;
  }

  // Parse the XML file
  xmlDocPtr doc = xmlReadFile(filename.c_str(), nullptr, 0);
  if (!doc) {
    LOG(LL::Error, "Could not parse XML file from ", filename);
    xmlSchemaFreeValidCtxt(valid_ctxt);
    xmlSchemaFree(schema);
    return nullptr;
  }

  // Validate the XML file
  int ret = xmlSchemaValidateDoc(valid_ctxt, doc);
  if(ret > 0){
    LOG(LL::Error, filename, " XML file is invalid according to ", scene_validation_file);
    return nullptr;
  } else if (ret < 0) {
    LOG(LL::Error, "Validation has generated an internal eror");
    return nullptr;
  }

  xmlNodePtr root = xmlDocGetRootElement(doc);
  std::shared_ptr<Scene> scene(new Scene);

  create_scene_nodes(scene, root);

  // Cleanup
  xmlFreeDoc(doc);
  xmlSchemaFreeValidCtxt(valid_ctxt);
  xmlSchemaFree(schema);
  xmlCleanupParser();

  return scene;
}

void SceneLoader::dumpScene(std::shared_ptr<Scene> scene, std::string filename){
  std::ofstream out_file(filename);

  if(!out_file.is_open()){
    LOG(LL::Error, "Could not open output file in sceneloader: ", filename, '.');
    throw std::runtime_error(filename+" could not be opened for write.");
  }

  out_file << scene->getRepresentation();
  out_file.close();

  LOG(LL::Info, "Scene write to file ", filename, " was successful.");
}


// Recursively load all of the children nodes.
void create_scene_nodes(std::shared_ptr<Scene> scene, xmlNodePtr& xml_node){

  std::vector<std::shared_ptr<Camera>>& cameras = scene->getCameras();
  std::vector<std::shared_ptr<Light>>& lights = scene->getLights();
  std::vector<std::shared_ptr<Asset>>& assets = scene->getAssets();
  std::shared_ptr<Entity> entity = nullptr;
  bool is_root = false;
  if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "scene")){
    // the entity is already set, do nothing.
    is_root = true;
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "camera")){
    std::shared_ptr<Camera> camera = create_camera_entity(xml_node);
    cameras.push_back(camera);
    entity = std::static_pointer_cast<Entity>(camera);
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "point_light")){
    std::shared_ptr<Light> light = create_pointlight_entity(xml_node);
    lights.push_back(light);
    entity = std::static_pointer_cast<Entity>(light);
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "spot_light")){
    std::shared_ptr<Light> light = create_spotlight_entity(xml_node);
    lights.push_back(light);
    entity = std::static_pointer_cast<Entity>(light);
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "directional_light")){
    std::shared_ptr<Light> light = create_directionallight_entity(xml_node);
    lights.push_back(light);
    entity = std::static_pointer_cast<Entity>(light);
  }
  else if(xml_node->type == XML_ELEMENT_NODE 
      && xmlStrEqual(xml_node->name, BAD_CAST "asset")){
    std::shared_ptr<Asset> asset = create_asset_entity(xml_node);
    assets.push_back(asset);
    entity = std::static_pointer_cast<Entity>(asset);
  }

  // Check if any of the child nodes are components
  for (xmlNodePtr cur = xml_node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "transformation")) {
      // add a transformation component to this node.
      std::shared_ptr<TransformComponent> transform;
      transform = create_transform(cur);

      // register the component.
      if(transform != nullptr){
        component_manager->addTransformComponent(entity->getId(), transform); 
      }

      continue;
    }
    else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "mesh")) {

      // add a mesh component to the entity
      std::shared_ptr<MeshComponent> mesh;
      mesh = create_mesh(cur);

      if(mesh != nullptr){
        component_manager->addMeshComponent(entity->getId(), mesh); 
      }
      continue;
    }
    else if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "shader")) {

      // add a mesh component to the entity
      std::shared_ptr<ShaderComponent> shader;
      shader = create_shader(cur);

      if(shader != nullptr){
        component_manager->addShaderComponent(entity->getId(), shader); 
      }
      continue;
    }

    create_scene_nodes(scene, cur);
  }
}

std::shared_ptr<Camera> create_camera_entity(xmlNodePtr& node){
  /*<camera name="camera_1" fov="60" buffer_x="100" buffer_y="100" z_near="1" z_far="10" projection_type="perspective">
    <vec3 name="up" x="0" y="1" z="0"></vec3>
    <vec3 name="lookat" x="1" y="0" z="0"></vec3>
    <vec3 name="right" x="0" y="0" z="1"></vec3>
    <vec3 name="position" x="0.2" y="0.2" z="0.2"></vec3>
  </camera>*/
  /*
  float fov;
  float buffer_x, buffer_y;
  float z_near, z_far;
  std::string projection_type;
  glm::vec3 up, lookat, position;

  // get the children nodes
  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "up")) {
      up = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "lookat")) {
      lookat = load_vec3(cur);
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

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(up, lookat, position, type);
  camera->setFov(fov);
  camera->setBufferDimensions(buffer_x, buffer_y);
  camera->setClippingPlane(z_near, z_far);

  // Free the attribute values
  xmlFree(fov_cstr);
  xmlFree(buffer_x_cstr);
  xmlFree(buffer_y_cstr);
  xmlFree(z_near_cstr);
  xmlFree(z_far_cstr);
  xmlFree(projection_type_cstr);

  return camera;

}

std::shared_ptr<Light> create_directionallight_entity(xmlNodePtr& node){

  glm::vec3 color, direction;
  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "color")) {
      color = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "direction")) {
      direction = load_vec3(cur);
    }
  }

  std::shared_ptr<DirectionalLight> directional_light = 
    light_manager->addDirectionalLight(color, direction);

  return directional_light;

}

std::shared_ptr<Light> create_spotlight_entity(xmlNodePtr& node){

  float attenuation, angle;
  std::string atteuation_type;
  glm::vec3 color, position, direction;
  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "color")) {
      color = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "position")) {
      position = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "direction")) {
      direction = load_vec3(cur);
    }
  }

  xmlChar *att_cstr = xmlGetProp(node, BAD_CAST "attenuation");
  xmlChar *angle_cstr = xmlGetProp(node, BAD_CAST "angle");
  xmlChar *att_t_cstr = xmlGetProp(node, BAD_CAST "attenuation_type");

  // Convert xmlChar* to std::string
  std::string att(reinterpret_cast<const char*>(att_cstr));
  std::string angle_str(reinterpret_cast<const char*>(angle_cstr));
  std::string att_t(reinterpret_cast<const char*>(att_t_cstr));

  // Convert string to float
  attenuation = std::stof(att);  
  angle = std::stof(angle_str);
  LightAttenuationType type;
  if(att_t == "constant"){
    type = LightAttenuationType::Constant;
  }
  else if(att_t == "linear"){
    type = LightAttenuationType::Linear;
  }
  else{
    type = LightAttenuationType::Quadratic;
  }

  std::shared_ptr<SpotLight> spot_light = light_manager->addSpotLight(
      color, attenuation, type, position, direction, angle);

  // Free the attribute values
  xmlFree(att_cstr);
  xmlFree(angle_cstr);
  xmlFree(att_t_cstr);

  return spot_light;

}

std::shared_ptr<Light> create_pointlight_entity(xmlNodePtr& node){
  */
  /*<point_light attenuation="1" attenuation_type="linear">
    <vec3 name="color" x="1" y="1" z="1"></vec3>
    <vec3 name="position" x="0" y="0" z="-10"></vec3>
  </point_light>*/
    /*

  float attenuation;
  std::string atteuation_type;
  glm::vec3 color, position;
  for (xmlNodePtr cur = node->children; cur != NULL; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "color")) {
      color = load_vec3(cur);
    }
    if (cur->type == XML_ELEMENT_NODE && xmlStrEqual(cur->name, BAD_CAST "position")) {
      position = load_vec3(cur);
    }
  }

  xmlChar *att_cstr = xmlGetProp(node, BAD_CAST "attenuation");
  xmlChar *att_t_cstr = xmlGetProp(node, BAD_CAST "attenuation_type");

  // Convert xmlChar* to std::string
  std::string att(reinterpret_cast<const char*>(att_cstr));
  std::string att_t(reinterpret_cast<const char*>(att_t_cstr));

  // Convert string to float
  attenuation = std::stof(att);  
  LightAttenuationType type;
  if(att_t == "constant"){
    type = LightAttenuationType::Constant;
  }
  else if(att_t == "linear"){
    type = LightAttenuationType::Linear;
  }
  else{
    type = LightAttenuationType::Quadratic;
  }

  std::shared_ptr<PointLight> point_light = light_manager->addPointLight(
                        color, attenuation, type, position);

  // Free the attribute values
  xmlFree(att_cstr);
  xmlFree(att_t_cstr);

  return point_light;

}

std::shared_ptr<Asset> create_asset_entity(xmlNodePtr& node){
  std::shared_ptr<Asset> asset = std::make_shared<Asset>();
  return asset;
}

std::shared_ptr<TransformComponent> create_transform(xmlNodePtr& node){
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
    transform_component->setTransform(transform);
    return transform_component;
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
    if(has_translate){
      transform_component->translate(translate);
    }
    if(has_rotate){
      transform_component->rotate(glm::vec3(rotate.x, rotate.y, rotate.z), rotate.w);
    }
    if(has_scale){
      transform_component->scale(scale);
    }

    return transform_component;
  }
}

std::shared_ptr<MeshComponent> create_mesh(xmlNodePtr& node){
  std::string filename = get_name(node); 
  return MeshLoader::loadMeshComponent(filename);
}

std::shared_ptr<ShaderComponent> create_shader(xmlNodePtr& node){
  xmlChar *vertex_cstr = xmlGetProp(node, BAD_CAST "vertex");
  xmlChar *fragment_cstr = xmlGetProp(node, BAD_CAST "fragment");

  // Convert xmlChar* to std::string
  std::string vertex_str(reinterpret_cast<const char*>(vertex_cstr));
  std::string fragment_str(reinterpret_cast<const char*>(fragment_cstr));

  xmlFree(fragment_cstr);
  xmlFree(vertex_cstr);

  return ShaderLoader::loadShaderComponent(vertex_str, fragment_str);
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

bool is_named(xmlNodePtr& node, std::string name){
  return get_name(node) == name;
}

std::string get_name(xmlNodePtr& node){
  xmlChar *name_cstr = xmlGetProp(node, BAD_CAST "name");
  std::string name_str(reinterpret_cast<const char*>(name_cstr));
  xmlFree(name_cstr);
  return name_str;
}

*/
void SceneLoader::load(Scene& scene, std::string filename){

}
