#include "engine.h"
#include <iostream>
#include "logger.h"

int main(void){
  //LOGGER.setLogLevel(LL::Verbose);
  Engine engine;

  engine.init();

  engine.loadScene("basic.xml");
  engine.execute();
  engine.shutdown();

  return 0;

}
