#include "./entity.hpp"

#include <algorithm>

// ====================
// Rabbit

Rabbit::Rabbit(Vec2 pos) : Entity() {
  position_ = pos;
}

void Rabbit::Draw(/*GUI& gui*/) {
  // FIXME: do GUI class with DrawRabbit & DrawSnake (basically, draw every game element)
  return;
}

bool Rabbit::Occupies(Vec2 tile) {
  return position_ == tile;
}

// ====================
// Snake

Snake::Snake(std::vector<Vec2> segments) : Entity() {
  segments_ = segments;

  move_direction_ = Direction::Up;
}

Vec2 Snake::Grow() {
  if (segments_.size() == 1) { // only head
    switch (move_direction_) {
    case Direction::Up:
      segments_.push_back(segments_.front() + ELEM_VECTOR_Y);
      break;
    case Direction::Down:
      segments_.push_back(segments_.front() - ELEM_VECTOR_Y);
      break;
    case Direction::Right:
      segments_.push_back(segments_.front() + ELEM_VECTOR_X);
      break;
    case Direction::Left:
      segments_.push_back(segments_.front() - ELEM_VECTOR_X);
      break;
    }
  } else {
    Vec2 ultimate    = segments_.back();
    Vec2 penultimate = segments_.end()[-2];

    if (ultimate.x == penultimate.x) {
      if (ultimate.y < penultimate.y)
        segments_.push_back(ultimate - ELEM_VECTOR_Y);
      else
        segments_.push_back(ultimate + ELEM_VECTOR_Y);
    } else if (ultimate.y == penultimate.y) {
      if (ultimate.x < penultimate.x)
        segments_.push_back(ultimate - ELEM_VECTOR_X);
      else
        segments_.push_back(ultimate + ELEM_VECTOR_X);
    }
  }

  return segments_.back();
}

Vec2 Snake::GetNewHeadPos() {
  switch (move_direction_) {
  case Direction::Up:
    return segments_.front() + ELEM_VECTOR_Y;
  case Direction::Down:
    return segments_.front() - ELEM_VECTOR_Y;
  case Direction::Right:
    return segments_.front() + ELEM_VECTOR_X;
  case Direction::Left:
    return segments_.front() - ELEM_VECTOR_X;
  }
}

Vec2 Snake::Move() {
  segments_.front() = GetNewHeadPos();

  std::cout << segments_.front() << "\n";

  for (auto i = segments_.rbegin(); i != segments_.rend() - 1; ++i)
    *i = *(i - 1);

  return segments_.front();
}

void Snake::Draw(/*GUI& gui*/) {
  // FIXME: do GUI class with DrawRabbit & DrawSnake (basically, draw every game element)
  return;
}

bool Snake::Occupies(Vec2 tile) {
  auto res = std::find(segments_.begin(), segments_.end(), tile);

  return res != segments_.end();
}
