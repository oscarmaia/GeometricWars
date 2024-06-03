#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager {
  EntityVector m_entities;
  EntityMap m_entityMap;
  size_t m_totalEntities;
  EntityVector m_toAdd;

 public:
  EntityManager();
  void update();
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  EntityVector& getEntities();
  EntityVector& getEntities(const std::string& tag);
};
