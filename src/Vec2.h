#pragma once

#include <SFML/System/Vector2.hpp>

class Vec2 {
 public:
  float x;
  float y;
  Vec2();
  Vec2(float xin, float yin);
  bool operator==(const Vec2& other) const;
  bool operator!=(const Vec2& other) const;

  Vec2 operator+(const Vec2& other) const;
  Vec2 operator-(const Vec2& other) const;
  Vec2 operator*(const float val) const;
  Vec2 operator/(const float val) const;
  float size(const Vec2& other) const;

  void operator+=(const Vec2& other);
  void operator-=(const Vec2& other);
  void operator*=(const Vec2& other);
  void operator/=(const Vec2& other);

  operator sf::Vector2f() const;
  Vec2 normalized(const Vec2& other) const;
};
