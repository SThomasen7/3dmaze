#include "engine.h"
#include <iostream>

int main(void){
  Engine engine;

  engine.init();
  engine.execute();
  engine.shutdown();

  return 0;

}
