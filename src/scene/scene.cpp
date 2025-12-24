#include "scene.h"
#include "logger.h"

Scene::Scene(){ active_camera = -1; }

EntityManager& Scene::getEntityManager(){
  return entity_manager;
}


void Scene::clear(){
  LOG(LL::Info, "Clearing the scene");
  entity_manager.clear();
  active_camera = -1;
}
