#include "entity_test.h"
#include "entity_allocator.h"
#include "entity_manager.h"
#include <iostream>
#include <memory>


MAKE_TEST(InitTest){
  TEST_START();
  EntityManager entity_manager;
  LOG << "Testing incremental IDs for entities" << std::endl;

  Entity entity = entity_manager.createEntity();
  uint32_t first_id = entity.id;
  CHECK_EQ(entity.id, first_id);

  Entity entity2 = entity_manager.createEntity();
  CHECK_EQ(entity2.id, first_id+1);
  Entity entity3(entity2);
  CHECK_EQ(entity3.id, first_id+1);
  CHECK_EQ(entity2.id, first_id+1);
  Entity entity4 = entity_manager.createEntity();
  CHECK_EQ(entity4.id, first_id+2);
}

MAKE_TEST(BasicIterationTest){
  TEST_START();
  EntityManager entity_manager;
  entity_manager.clear();

  Entity entity1 = entity_manager.createEntity();
  uint32_t first_id = entity1.id;
  Entity entity2 = entity_manager.createEntity();
  Entity entity3 = entity_manager.createEntity();

  uint32_t idx = 0;
  for(auto it : entity_manager){
    CHECK_EQ(it.id, idx+first_id);
    idx+=1;
  }

}

MAKE_TEST(PositionComponentTest){
  TEST_START();
  EntityManager entity_manager;
  entity_manager.clear();

  Entity entity1 = entity_manager.createEntity();
  uint32_t first_id = entity1.id;
  entity_manager.addPositionComponent(entity1, { glm::vec3(static_cast<float>(first_id)) });
  Entity entity2 = entity_manager.createEntity();
  entity_manager.addPositionComponent(entity2, { glm::vec3(static_cast<float>(first_id+1)) });
  Entity entity3 = entity_manager.createEntity();
  entity_manager.addPositionComponent(entity3, { glm::vec3(static_cast<float>(first_id+2)) });

  uint32_t idx = 0;
  for(auto it : entity_manager){
    CHECK_EQ(it.id, idx+first_id);

    PositionComponent position = entity_manager.getPosition(it);
    float pos = position.position.x;
    CHECK_EQ(pos, idx+first_id);

    idx+=1;
  }
}

MAKE_TEST(ViewListTest){
  TEST_START();
  EntityManager entity_manager;
  entity_manager.clear();

  Entity entity1 = entity_manager.createEntity();
  Entity entity2 = entity_manager.createEntity();
  Entity entity3 = entity_manager.createEntity();
  Entity entity4 = entity_manager.createEntity();
  uint32_t first_id = entity1.id;

  entity_manager.addPositionComponent(entity2, { glm::vec3(static_cast<float>(first_id+1)) });
  entity_manager.addPositionComponent(entity4, { glm::vec3(static_cast<float>(first_id+2)) });
  
  // Create the view
  using EntityView = EntityManager::EntityView;
  EntityView* eview = 
    entity_manager.createEntityView<PositionComponent>();

  for(auto it = eview->begin(); it != eview->end(); it++){
    bool matches = (*it == entity2 || *it == entity4);
    bool not_matches = (*it != entity1 && *it != entity3);

    CHECK_TRUE(matches);
    CHECK_TRUE(not_matches);
  }

  entity_manager.destroyEntityView(&eview);
  CHECK_TRUE(eview == nullptr);
}


MAKE_TEST(EntityTypesTest){
  TEST_START();

  EntityManager entity_manager;
  entity_manager.clear();
  Entity entity_position = entity_manager.createEntity();
  Entity entity_transform = entity_manager.createEntity();
  Entity entity_shader = entity_manager.createEntity();
  Entity entity_mesh = entity_manager.createEntity();
  Entity entity_light = entity_manager.createEntity();
  Entity entity_light_angle = entity_manager.createEntity();
  Entity entity_light_direction = entity_manager.createEntity();
  Entity entity_camera = entity_manager.createEntity();
  Entity entity_render = entity_manager.createEntity();

  entity_manager.addPositionComponent(entity_position, {});
  entity_manager.addTransformComponent(entity_transform, {});
  entity_manager.addShaderComponent(entity_shader, {});
  entity_manager.addMeshComponent(entity_mesh, {});
  entity_manager.addLightComponent(entity_light, {});
  entity_manager.addLightDirectionComponent(entity_light_direction, {});
  entity_manager.addLightAngleComponent(entity_light_angle, {});
  entity_manager.addCameraComponent(entity_camera, {});
  entity_manager.addRenderComponent(entity_render, {});

  try{
    entity_manager.getPosition(entity_position);
  } catch(...){
    LOG << "Failed to get position entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getTransform(entity_transform);
  } catch(...){
    LOG << "Failed to get transform entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getShader(entity_shader);
  } catch(...){
    LOG << "Failed to get shader entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getMesh(entity_mesh);
  } catch(...){
    LOG << "Failed to get mesh entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getLight(entity_light);
  } catch(...){
    LOG << "Failed to get light entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getLightAngle(entity_light_angle);
  } catch(...){
    LOG << "Failed to get light angle entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getLightDirection(entity_light_direction);
  } catch(...){
    LOG << "Failed to get light direction entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getCamera(entity_camera);
  } catch(...){
    LOG << "Failed to get camera entity." << std::endl;
    CHECK_TRUE(false);
  }

  try{
    entity_manager.getRender(entity_render);
  } catch(...){
    LOG << "Failed to get render entity." << std::endl;
    CHECK_TRUE(false);
  }

}

void set_entity_tests(){
  TEST(EntityTestSuite, InitTest);
  TEST(EntityManagerSuite, BasicIterationTest);
  TEST(EntityManagerSuite, PositionComponentTest);
  TEST(EntityManagerSuite, ViewListTest);
  TEST(EntityManagerSuite, EntityTypesTest);
}
