#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_ 1

#include <vector>
#include <initializer_list>
#include <typeindex>
#include "component_pool.h"
#include "position_component.h"
#include "transform_component.h"
#include "shader_component.h"
#include "mesh_component.h"
#include "light_component.h"
#include "light_direction_component.h"
#include "light_angle_component.h"

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

  // Specific Component functions
  void addPositionComponent(const Entity& entity, PositionComponent position);
  PositionComponent& getPosition(Entity& entity);
  PositionComponent getPosition(const Entity& entity) const;

  void addTransformComponent(const Entity& entity, TransformComponent transform);
  TransformComponent& getTransform(Entity& entity);
  TransformComponent getTransform(const Entity& entity) const;

  void addShaderComponent(const Entity& entity, ShaderComponent shader);
  ShaderComponent& getShader(Entity& entity);
  ShaderComponent getShader(const Entity& entity) const;

  void addMeshComponent(const Entity& entity, MeshComponent mesh);
  MeshComponent& getMesh(Entity& entity);
  MeshComponent getMesh(const Entity& entity) const;

  void addLightComponent(const Entity& entity, LightComponent light);
  LightComponent& getLight(Entity& entity);
  LightComponent getLight(const Entity& entity) const;

  void addLightDirectionComponent(const Entity& entity, LightDirectionComponent light_direction);
  LightDirectionComponent& getLightDirection(Entity& entity);
  LightDirectionComponent getLightDirection(const Entity& entity) const;

  void addLightAngleComponent(const Entity& entity, LightAngleComponent light_angle);
  LightAngleComponent& getLightAngle(Entity& entity);
  LightAngleComponent getLightAngle(const Entity& entity) const;

  // Iterator functions
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
  ComponentPool<TransformComponent> transform_pool;
  ComponentPool<ShaderComponent> shader_pool;
  ComponentPool<MeshComponent> mesh_pool;
  ComponentPool<LightComponent> light_pool;
  ComponentPool<LightDirectionComponent> light_direction_pool;
  ComponentPool<LightAngleComponent> light_angle_pool;

};

#endif
