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
  for (auto e : m_toAdd) {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  for (auto it = m_entities.begin(); it != m_entities.end();) {
    if (!(*it)->isActive()) {
      m_entityMap.erase((*it)->tag());
      it = m_entities.erase((it));
    } else {
      it++;
    }
    // std::remove_if(m_entities.begin(),m_entities.end(),e.isac)
  }
  m_toAdd.clear();
};
