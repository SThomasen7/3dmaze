#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_ 1

#include "system.h"

class RenderSystem : public System{

public:

  RenderSystem();

  void init() override;
  void process() override;
  void shutdown() override;


};

#endif

