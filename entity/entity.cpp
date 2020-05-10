#include "./entity.hpp"

#include <algorithm>

// ====================
// Entity

Entity::Entity(EntityKind kind, bool status = false) {
  status_ = status;
  kind_   = kind;
}

EntityKind Entity::GetEntityKind() {
  return kind_;
}

// ====================
// Rabbit

Rabbit::Rabbit(Vec2 pos, bool status = false) : Entity(EntityKind::Rabbit, status_) {
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

Snake::Snake(std::vector<Vec2> segments, bool status = false) : Entity(EntityKind::Snake, status_) {
  segments_ = segments;

  move_direction_ = Direction::Neutral;
}

Vec2 Snake::Grow() {
  if (segments_.size() == 1) { // only head
    switch (move_direction_) {
    case Direction::Neutral:
      break;
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

Vec2 Snake::Move() {
  for (auto i = segments_.rbegin(); i != segments_.rend() - 1; ++i)
    *i = *(i - 1);

  switch (move_direction_) {
  case Direction::Up:
    segments_.front() += ELEM_VECTOR_Y;
    break;
  case Direction::Down:
    segments_.front() -= ELEM_VECTOR_Y;
    break;
  case Direction::Right:
    segments_.front() += ELEM_VECTOR_X;
    break;
  case Direction::Left:
    segments_.front() -= ELEM_VECTOR_X;
    break;
  }

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
