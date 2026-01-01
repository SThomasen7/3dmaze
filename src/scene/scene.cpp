#include "scene.h"
#include "logger.h"

Scene::Scene(){ 
  active_camera = -1; 
  dispatcher = nullptr;
}

EntityManager& Scene::getEntityManager(){
  return entity_manager;
}


void Scene::clear(){
  LOG(LL::Info, "Clearing the scene");
  entity_manager.clear();
  active_camera = -1;
}

void Scene::setEventDispatcher(EventDispatcher* dispatcher){
  this->dispatcher = dispatcher;
  dispatcher->subscribe<ResizeScreenEvent>(
    std::bind(&Scene::handleScreenResizeEvent, this, std::placeholders::_1));
}

void Scene::handleScreenResizeEvent(const ResizeScreenEvent &event){
  LOG(LL::Verbose, "Resizing screen from scene");

  using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<CameraComponent>();

  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    CameraComponent& component = entity_manager.getCamera(*entity_ptr);
    component.buffer_x = event.buffer_x;
    component.buffer_y = event.buffer_y;
  }
  entity_manager.destroyEntityView(&eview);
}
