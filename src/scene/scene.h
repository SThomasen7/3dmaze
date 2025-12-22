#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include "entity_manager.h"

class Scene{

public:

  Scene();

  EntityManager& getEntityManager();
  void clear();

private:


  EntityManager entity_manager;

};


#endif
