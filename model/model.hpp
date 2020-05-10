#pragma once

// #include "../utils/direction.hpp"
#include "../entity/entity.hpp"
#include "../utils/log.hpp"
#include "../utils/response.hpp"
#include "../utils/vec2.hpp"

#include <array>
#include <set>
#include <vector>

const int MAX_RABBITS  = 10;
const int MAX_SNAKES   = 4;
const int MAX_ENTITIES = MAX_RABBITS + MAX_SNAKES;

typedef int EntityId;

class Model {
public:
  Model(Vec2 map_size, int n_snakes, int n_rabbits);
  ~Model() = default;

  void Tick();

  ResponseCode SetSnakeMoveDirection(EntityId snake);
  Direction    GetSnakeMoveDirection(EntityId snake);

  EntityId GetPlayerSnakeId();

private:
  EntityId AddSnake(Vec2 pos);
  EntityId AddRabbit(Vec2 pos);

  ResponseCode RemoveEntity(EntityId id);

  ResponseCode MoveSnake(EntityId id);
  void         GrowSnake(EntityId id);

  Vec2 map_size;

  int                               n_rabbits;
  int                               n_snakes_;
  EntityId                          player_snake_;
  std::array<Entity*, MAX_ENTITIES> entities_;
};