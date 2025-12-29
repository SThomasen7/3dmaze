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
  input_system.setupEntityManager(scene.getEntityManager());
  float start_time = glfwGetTime();
  float previous_time = glfwGetTime();
  float accumulator = 0.0f;
  float FRAME_RATE = 1.0f/1000.0f;

  while(!settings.should_close){

    float current_time = glfwGetTime();
    float delta_time = current_time - previous_time;
    previous_time = current_time;

    accumulator += delta_time;
    input_system.pollEvents(settings);
    window_manager.pollEvents(settings);
    window_manager.preRender();

    input_system.process(scene, delta_time);
    if(accumulator >= FRAME_RATE){
      physics_system.process(scene, accumulator);
      render_system.process(scene, accumulator);
      accumulator = 0.0f;
    }

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
