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

float Vec2::magnitude() const {
  return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalize() const {
  float magnitude = Vec2(x, y).magnitude();
  return Vec2(x, y) / magnitude;
}
