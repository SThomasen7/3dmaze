#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_ 1

#include <unordered_map>
#include <memory>

#include "logger.h"

template<typename T>
class ResourceManager{

  std::unordered_map<std::string, T> items;

public:

  ResourceManager(){}

  inline void load(const std::string &key, T item){
    if(isLoaded(key)){
      LOG(LL::Warn, "Resource with key ", key, " already loaded, rewriting");
    }
    items[key] = item; 
  }

  inline bool isLoaded(const std::string &key) const{
    if(items.find(key) != items.end()){
      return true;
    }
    return false;
  }
  
  inline std::shared_ptr<T> get(const std::string &key){
    auto it = items.find(key);
    if(it != items.end()){
      return std::make_shared<T>(it->second);
    }
    return nullptr;
  }

};

#endif
