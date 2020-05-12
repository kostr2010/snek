#pragma once

#include <cmath>
#include <iostream>
#include <vector>

// ====================
// Vec2
// implementation of the simple 2d vector

struct Vec2 {
  Vec2() = default;
  Vec2(int i, int j);
  Vec2(const Vec2& that);

  double Length();

  int x, y;
};

bool operator==(Vec2 left, Vec2 right);

void operator+=(Vec2& left, const Vec2& right);

Vec2 operator+(const Vec2 left, const Vec2 right);

void operator-=(Vec2& left, const Vec2& right);

Vec2 operator-(const Vec2 left, const Vec2 right);

std::ostream& operator<<(std::ostream& os, Vec2 v);

std::ostream& operator<<(std::ostream& os, std::vector<Vec2> v);

void operator*=(Vec2& left, const int& right);

Vec2 operator*(const Vec2 left, int right);