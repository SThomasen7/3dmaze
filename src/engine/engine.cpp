#include "engine.h"
#include "logger.h"
#include "scene_loader.h"

Engine::Engine(){
  settings = {
    false
  };
}


void Engine::execute(){

  // Init systems
  render_system.init();

  while(!settings.should_close){
    window_manager.pollEvents(settings);
    window_manager.preRender();

    render_system.process();

    window_manager.postRender();
  }

  // Shutdown systems
  render_system.shutdown();

}

void Engine::init(){
  LOG(LL::Info, "Initializing engine");
  window_manager.init();
}

void Engine::shutdown(){
  LOG(LL::Info, "Shutting down engine");
  window_manager.shutdown();
}

void Engine::loadScene(std::string filename){
  SceneLoader::load(scene, filename);
}
