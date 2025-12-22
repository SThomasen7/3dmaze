#include "logger.h"
#include "render_system.h"


RenderSystem::RenderSystem(){ }

void RenderSystem::init(){
  LOG(LL::Info, "Initializing Render System.");
}

void RenderSystem::process(){

}

void RenderSystem::shutdown(){
  LOG(LL::Info, "Shutting down Render System.");
}

