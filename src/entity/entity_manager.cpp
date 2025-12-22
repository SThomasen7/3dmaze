#include "entity_manager.h"

using std::initializer_list;
using std::type_index;

EntityManager::EntityManager() {}

Entity EntityManager::createEntity(){
  Entity entity;
  entity_list.push_back(entity);
  return entity;
}

EntityManager::iterator EntityManager::begin(){
  return entity_list.begin();
}

EntityManager::iterator EntityManager::end(){
  return entity_list.end();
}

EntityManager::const_iterator EntityManager::begin() const{
  return entity_list.begin();
}

EntityManager::const_iterator EntityManager::end() const{
  return entity_list.end();
}

void EntityManager::clear(){
  entity_list.clear();
  position_pool.clear();
}

EntityManager::EntityView* EntityManager::createEntityViewImpl(initializer_list<type_index> types){
  std::vector<Entity> view_list; 
  for(auto entity : entity_list){
    bool found = true;

    // For each of the specified types, check if the entity has that component
    for(auto arg_type_id : types){

      // Check that the entity has all of the component types specified
      if(arg_type_id == std::type_index(typeid(PositionComponent))){
        if(!position_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(TransformComponent))){
        if(!transform_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(ShaderComponent))){
        if(!shader_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(MeshComponent))){
        if(!mesh_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(LightComponent))){
        if(!light_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(LightDirectionComponent))){
        if(!light_direction_pool.contains(entity)){
          found = false;
          break;
        }
      }
      else if(arg_type_id == std::type_index(typeid(LightAngleComponent))){
        if(!light_angle_pool.contains(entity)){
          found = false;
          break;
        }
      }

    }
    if(found){
      view_list.push_back(entity);
    }
  }

  EntityManager::EntityView* view_ptr = new EntityManager::EntityView(view_list);
  return view_ptr;
}

void EntityManager::destroyEntityView(EntityManager::EntityView** entity_view){
  if(*entity_view){
    delete *entity_view;
  }
  *entity_view = nullptr;
}


EntityManager::EntityView::EntityView(std::vector<Entity> entity_list) : view_list(entity_list){ }


EntityManager::iterator EntityManager::EntityView::begin(){
  return view_list.begin();
}

EntityManager::iterator EntityManager::EntityView::end(){
  return view_list.end();
}

EntityManager::const_iterator EntityManager::EntityView::begin() const{
  return view_list.begin();
}

EntityManager::const_iterator EntityManager::EntityView::end() const{
  return view_list.end();
}


void EntityManager::addPositionComponent(const Entity& entity, PositionComponent position){
  position_pool[entity] = position;
}

PositionComponent& EntityManager::getPosition(Entity& entity){
  return position_pool.at(entity);
}

PositionComponent EntityManager::getPosition(const Entity& entity) const{
  return position_pool.at(entity);
}

void EntityManager::addTransformComponent(const Entity& entity, TransformComponent transform){
  transform_pool[entity] = transform;
}

TransformComponent& EntityManager::getTransform(Entity& entity){
  return transform_pool.at(entity);
}

TransformComponent EntityManager::getTransform(const Entity& entity) const{
  return transform_pool.at(entity);
}


void EntityManager::addShaderComponent(const Entity& entity, ShaderComponent shader){
  shader_pool[entity] = shader;
}

ShaderComponent& EntityManager::getShader(Entity& entity){
  return shader_pool.at(entity);
}

ShaderComponent EntityManager::getShader(const Entity& entity) const{
  return shader_pool.at(entity);
}


void EntityManager::addMeshComponent(const Entity& entity, MeshComponent mesh){
  mesh_pool[entity] = mesh;
}

MeshComponent& EntityManager::getMesh(Entity& entity){
  return mesh_pool.at(entity);
}

MeshComponent EntityManager::getMesh(const Entity& entity) const{
  return mesh_pool.at(entity);
}


void EntityManager::addLightComponent(const Entity& entity, LightComponent light){
  light_pool[entity] = light;
}

LightComponent& EntityManager::getLight(Entity& entity){
  return light_pool.at(entity);
}

LightComponent EntityManager::getLight(const Entity& entity) const{
  return light_pool.at(entity);
}


void EntityManager::addLightDirectionComponent(
    const Entity& entity, LightDirectionComponent light_direction){
  light_direction_pool[entity] = light_direction;
}

LightDirectionComponent& EntityManager::getLightDirection(Entity& entity){
  return light_direction_pool.at(entity);
}

LightDirectionComponent EntityManager::getLightDirection(const Entity& entity) const{
  return light_direction_pool.at(entity);
}


void EntityManager::addLightAngleComponent(
    const Entity& entity, LightAngleComponent light_angle){
  light_angle_pool[entity] = light_angle;
}

LightAngleComponent& EntityManager::getLightAngle(Entity& entity){
  return light_angle_pool.at(entity);
}

LightAngleComponent EntityManager::getLightAngle(const Entity& entity) const{
  return light_angle_pool.at(entity);
}

