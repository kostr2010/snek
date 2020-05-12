#include "./entity.hpp"
#include "../utils/log.hpp"

#include <algorithm>

// ====================
// Rabbit

Rabbit::Rabbit(Vec2 pos) : Entity() {
  position_ = pos;
}

bool Rabbit::Occupies(Vec2 tile) {
  return position_ == tile;
}

// ====================
// Snake

Snake::Snake(std::vector<Vec2> segments, int tag) : Entity() {
  segments_ = segments;
  score_    = 0;
  tag_      = tag;

  move_direction_ = Direction::Up;
}

Vec2 Snake::Grow() {
  LOG_LVL_MODEL_ROUTINE("trying to grow snake " << tag_ << " of size " << segments_.size() << ": "
                                                << segments_);

  Vec2 segm_new{};

  if (segments_.size() == 1) { // only head
    switch (move_direction_) {
    case Direction::Up:
      segm_new = segments_.front() + ELEM_VECTOR_Y;
      LOG_LVL_MODEL_ROUTINE("A attached segment at " << segm_new);
      break;
    case Direction::Down:
      segm_new = segments_.front() - ELEM_VECTOR_Y;
      LOG_LVL_MODEL_ROUTINE("V attached segment at " << segm_new);
      break;
    case Direction::Right:
      segm_new = segments_.front() - ELEM_VECTOR_X;
      LOG_LVL_MODEL_ROUTINE("> attached segment at " << segm_new);
      break;
    case Direction::Left:
      segm_new = segments_.front() + ELEM_VECTOR_X;
      LOG_LVL_MODEL_ROUTINE("< attached segment at " << segm_new);
      break;
    }
  } else {
    Vec2 ultimate    = segments_.back();
    Vec2 penultimate = segments_.end()[-2];

    if (ultimate.x == penultimate.x) {
      if (ultimate.y < penultimate.y) {
        segm_new = ultimate - ELEM_VECTOR_Y;
        LOG_LVL_MODEL_ROUTINE("attached segment at " << ultimate - ELEM_VECTOR_Y);

      } else {
        segm_new = ultimate + ELEM_VECTOR_Y;
        LOG_LVL_MODEL_ROUTINE("attached segment at " << ultimate + ELEM_VECTOR_Y);
      }
    } else if (ultimate.y == penultimate.y) {
      if (ultimate.x < penultimate.x) {
        segm_new = ultimate - ELEM_VECTOR_X;
        LOG_LVL_MODEL_ROUTINE("attached segment at " << ultimate - ELEM_VECTOR_X);
      } else {
        segm_new = ultimate + ELEM_VECTOR_X;
        LOG_LVL_MODEL_ROUTINE("attached segment at " << ultimate + ELEM_VECTOR_X);
      }
    }
  }

  segments_.push_back(segm_new);

  LOG_LVL_MODEL_ROUTINE("now this snake consists of " << segments_);

  return segments_.back();
}

Vec2 Snake::GetNewHeadPos() {
  switch (move_direction_) {
  case Direction::Up:
    return segments_.front() - ELEM_VECTOR_Y;
  case Direction::Down:
    return segments_.front() + ELEM_VECTOR_Y;
  case Direction::Right:
    return segments_.front() + ELEM_VECTOR_X;
  case Direction::Left:
    return segments_.front() - ELEM_VECTOR_X;
  default:
    return {-1, -1};
  }
}

Vec2 Snake::Move() {
  // segments_.front() = GetNewHeadPos();

  LOG_LVL_MODEL_ROUTINE("moving snake " << tag_ << ": " << segments_);

  for (int i = segments_.size() - 1; i > 0; i--)
    segments_[i] = segments_[i - 1];

  segments_.front() = GetNewHeadPos();

  LOG_LVL_MODEL_ROUTINE("moved snake " << tag_ << ": " << segments_);

  return segments_.front();
}

bool Snake::Occupies(Vec2 tile) {
  auto res = std::find(segments_.begin(), segments_.end(), tile);

  return res != segments_.end();
}
