#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager {
 public:
  EntityVector m_entities;
  EntityMap m_entityMap;
  size_t m_totalEntities;
  EntityVector m_toAdd;

  EntityManager();
  void update();
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  EntityVector& getEntities();
  EntityMap& getEntities(const std::string& tag);
};
