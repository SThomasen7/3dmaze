#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_ 1

#include "component_pool.h"
#include "position_component.h"
#include <vector>
#include <initializer_list>
#include <typeindex>

class EntityManager{

public:

  using container=std::vector<Entity>;
  using iterator=typename container::iterator;
  using const_iterator=typename container::const_iterator;

  class EntityView{
    private:
      friend EntityManager;
      EntityView(std::vector<Entity>);
    public:

      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;

    private:
      std::vector<Entity> view_list;

  };


  EntityManager();

  Entity createEntity();

  void addPositionComponent(const Entity& entity, PositionComponent position);
  PositionComponent& getPosition(Entity& entity);
  PositionComponent getPosition(const Entity& entity) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  void clear();
  
  // get the entity view
  template <typename ...Components>
  EntityView* createEntityView() {
      static_assert(sizeof...(Components) > 0);
      return createEntityViewImpl({ typeid(Components)... });
  }

  void destroyEntityView(EntityView** entity_view);

private:

  EntityView* createEntityViewImpl(std::initializer_list<std::type_index> types);

  std::vector<Entity> entity_list;
  ComponentPool<PositionComponent> position_pool;

};

#endif
