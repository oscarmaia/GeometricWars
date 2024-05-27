#include "Entity.h"
#include <cstddef>
#include <memory>

Entity::Entity(const std::string& tag, const size_t id)
    : m_tag(tag),
      m_id(id){
          // auto e = std::shared_ptr<Entity>(new Entity(tag, id));
      };

bool Entity::isActive() const {
  return m_alive;
}

void Entity::destroy() {
  m_alive = false;
}

const std::string& Entity::tag() const {
  return m_tag;
}

const size_t Entity::id() const {
  return m_id;
}
