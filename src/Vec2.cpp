#include "Vec2.h"
#include <math.h>
#include <SFML/System/Vector2.hpp>
#include <cmath>

Vec2::Vec2() {
  x = 0;
  y = 0;
}

Vec2::Vec2(float xin, float yin)
    : x(xin),
      y(yin) {}

Vec2 Vec2::operator+(const Vec2& other) const {
  return Vec2(x + other.x, y + other.y);
};

Vec2 Vec2::operator-(const Vec2& other) const {
  return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const {
  return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
  return Vec2(x / scalar, y / scalar);
}

bool Vec2::operator==(const Vec2& other) const {
  return (x == other.x && y == other.y);
}

bool Vec2::operator!=(const Vec2& other) const {
  return (x != other.x || y != other.y);
}

Vec2::operator sf::Vector2f() const {
  return sf::Vector2f(x, y);
}

float Vec2::size(const Vec2& other) const {
  Vec2 dif = other - Vec2(x, y);
  return std::sqrt(dif.x * dif.x + dif.y * dif.y);
}

Vec2 Vec2::normalized(const Vec2& other) const {
  Vec2 dif = other - Vec2(x, y);
  float size = Vec2(x, y).size(other);
  // TODO VERIFICAR SE ESTA CORRETO!
  return Vec2(dif.x / size, dif.y / size);
}
