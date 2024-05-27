#pragma once
#include <memory>
#include <string>
#include "Components.h"

class Entity {
 public:
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CName> cName;
  std::shared_ptr<CShape> cShape;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CScore> cScore;
  std::shared_ptr<CLifespan> cLifespan;
  std::shared_ptr<CInput> cInput;

  Entity(const std::string& tag, const size_t id);

  bool isActive() const;

  void destroy();
  const size_t id() const;
  const std::string& tag() const;

 private:
  const size_t m_id = 0;
  const std::string m_tag = "Default";
  bool m_alive = true;
};
