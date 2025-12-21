#include "entity_manager.h"

EntityManager ENTITY_MANAGER;

using std::initializer_list;
using std::type_index;

EntityManager::EntityManager() {}

Entity EntityManager::createEntity(){
  Entity entity;
  entity_list.push_back(entity);
  return entity;
}

void EntityManager::addPositionComponent(const Entity& entity, PositionComponent position){
  position_pool[entity] = position;
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

PositionComponent& EntityManager::getPosition(Entity& entity){
  return position_pool.at(entity);
}

PositionComponent EntityManager::getPosition(const Entity& entity) const{
  return position_pool.at(entity);
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

      // Position Component check
      if(arg_type_id == std::type_index(typeid(PositionComponent))){
        if(!position_pool.contains(entity)){
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

