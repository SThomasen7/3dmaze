#include "engine.h"
#include "logger.h"
#include "scene_loader.h"
#include "consts.h"

Engine::Engine(){
  settings = {
    false
  };
}


void Engine::execute(){

  // Init systems
  render_system.init();
  input_system.setupWindow(window_manager);
  input_system.init();
  physics_system.init();

  render_system.preLoadScene(scene);
  while(!settings.should_close){
    input_system.pollEvents(settings);
    window_manager.pollEvents(settings);
    window_manager.preRender();

    input_system.process(scene);
    physics_system.process(scene);
    render_system.process(scene);

    window_manager.postRender();
  }

  // Shutdown systems
  physics_system.shutdown();
  render_system.clearScene(scene);
  input_system.shutdown();
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
  SceneLoader::load(scene, asset_path+std::string("scenes/")+filename);
}
