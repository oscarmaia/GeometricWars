#include "Vec2.h"
#include <SFML/System/Vector2.hpp>

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

bool Vec2::operator!=(const Vec2& other) const {}

Vec2::operator sf::Vector2f() const {
  return sf::Vector2f(x, y);
}
