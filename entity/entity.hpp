#pragma once

#include "../utils/direction.hpp"
#include "../utils/vec2.hpp"

#include <vector>

enum class EntityKind {
  Snake,
  Rabbit,
};

class Entity {
public:
  Entity() = delete;
  Entity(EntityKind kind, bool status /* = false*/);

  ~Entity() = default;

  EntityKind GetEntityKind();

  virtual void Draw(/*GUI& gui*/)  = 0;
  virtual bool Occupies(Vec2 tile) = 0;

  bool status_;

protected:
  EntityKind kind_;
};

class Rabbit : public Entity {
  Rabbit() = delete;
  Rabbit(Vec2 pos, bool status /* = false*/);
  ~Rabbit() = default;

  void Draw(/*GUI& gui*/) override;
  bool Occupies(Vec2 tile) override;

  Vec2 position_;
};

class Snake : public Entity {
public:
  Snake() = delete;
  Snake(std::vector<Vec2> segments, bool status /* = false*/);
  ~Snake() = default;

  Vec2 Grow(); // returns coodrs of the newly grown segment
  Vec2 Move(); // returns new coords of the head

  void Draw(/*GUI& gui*/) override;
  bool Occupies(Vec2 pos) override;

private:
  Direction         move_direction_;
  std::vector<Vec2> segments_;
};