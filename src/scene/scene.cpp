#include "scene.h"
#include "logger.h"

Scene::Scene(){ }

EntityManager& Scene::getEntityManager(){
  return entity_manager;
}


void Scene::clear(){
  LOG(LL::Info, "Clearing the scene");
  entity_manager.clear();
}
