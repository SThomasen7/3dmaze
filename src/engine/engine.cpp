#include "engine.h"
#include "logger.h"
#include "scene_loader.h"
#include "consts.h"

Engine::Engine(){
  settings = {
    false,
    false
  };
}


void Engine::execute(){

  render_system.preLoadScene(scene);

  float start_time = glfwGetTime();
  float previous_time = glfwGetTime();
  float accumulator = 0.0f;
  float FRAME_RATE = 1.0f/60.0f;

  while(!settings.should_close){
    input_system.pollEvents(settings);
    window_manager.pollEvents(settings);

    float current_time = glfwGetTime();
    float delta_time = current_time - previous_time;
    previous_time = current_time;

    window_manager.preRender();
    // Don't render or process if we are paused
    if(settings.is_paused){
      window_manager.postRender();
      continue;
    }

    accumulator += delta_time;


    input_system.process(scene, delta_time);
    if(accumulator >= FRAME_RATE){
      physics_system.process(scene, accumulator);
      render_system.process(scene, accumulator);
      accumulator = 0.0f;
    }

    window_manager.postRender();
  }
}

void Engine::init(){
  LOG(LL::Info, "Initializing engine");
  window_manager.init();
  // Init systems
  render_system.init(&event_dispatcher);
  input_system.setupWindow(window_manager);
  input_system.init(&event_dispatcher);
  physics_system.init(&event_dispatcher);
  input_system.setupEntityManager(scene.getEntityManager());

  scene.setEventDispatcher(&event_dispatcher);

  event_dispatcher.subscribe<TogglePauseEvent>(
    std::bind(&Engine::handlePauseEvent, this, std::placeholders::_1));

}

void Engine::shutdown(){
  LOG(LL::Info, "Shutting down engine");
  // Shutdown systems
  physics_system.shutdown();
  render_system.clearScene(scene);
  input_system.shutdown();
  render_system.shutdown();
  window_manager.shutdown();
}

void Engine::loadScene(std::string filename){
  SceneLoader::load(scene, asset_path+std::string("scenes/")+filename);
}

void Engine::handlePauseEvent(const TogglePauseEvent& event){

    if(event.force){
      settings.is_paused = event.pause;
    }
    else{
      settings.is_paused = !settings.is_paused;
    }

    if(settings.is_paused){
      input_system.releaseCursor();
    }
    else{
      input_system.holdCursor();
    }
}
