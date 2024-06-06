#include "EntityManager.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "Entity.h"

EntityManager::EntityManager()
    : m_totalEntities(0) {}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
  auto e = std::make_shared<Entity>(tag, m_totalEntities++);
  m_toAdd.push_back(e);
  return e;
}

EntityVector& EntityManager::getEntities() {
  return m_entities;
}

EntityVector& EntityManager::getEntities(const std::string& tag) {
  return m_entityMap[tag];
}

void EntityManager::update() {
  for (auto& e : m_toAdd) {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  // auto newEnd = std::remove_if(
  //     m_entities.begin(), m_entities.end(), [](const std::shared_ptr<Entity>& e) { return !e->isActive(); });
  // m_entities.erase(newEnd, m_entities.end());

  std::erase_if(m_entities, [&](const std::shared_ptr<Entity>& e) {
    if (!e->isActive()) {
      std::erase(m_entityMap[e->tag()], e);
      return true;
    };
    return false;
  });
  m_toAdd.clear();
};
