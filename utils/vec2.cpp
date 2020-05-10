#include "./vec2.hpp"

Vec2::Vec2(int i, int j) {
  x = i;
  y = j;
}

Vec2::Vec2(const Vec2& that) {
  this->x = that.x;
  this->y = that.y;
}

bool operator==(Vec2 left, Vec2 right) {
  return (left.x == right.x) && (left.y == right.y);
}

void operator+=(Vec2& left, const Vec2& right) {
  left.x += right.x;
  left.y += right.y;
}

Vec2 operator+(const Vec2 left, const Vec2 right) {
  return {left.x + right.x, left.y + right.y};
}

void operator-=(Vec2& left, const Vec2& right) {
  left.x -= right.x;
  left.y -= right.y;
}

Vec2 operator-(const Vec2 left, const Vec2 right) {
  return {left.x - right.x, left.y - right.y};
}

std::ostream& operator<<(std::ostream& os, Vec2 v) {
  return os << "{" << v.x << ", " << v.y << "}";
}

void operator*=(Vec2& left, const int& right) {
  left.x *= right;
  left.y *= right;
}

Vec2 operator*(const Vec2 left, int right) {
  return {left.x * right, left.y * right};
}