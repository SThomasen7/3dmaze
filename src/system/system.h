#ifndef _SYSTEM_H_
#define _SYSTEM_H_ 1

class System{

public:
  System() {};

  virtual void init() = 0;
  virtual void process() = 0;
  virtual void shutdown() = 0;

};

#endif
