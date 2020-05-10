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

const int MIN_MAP_X = 20;
const int MIN_MAP_Y = 20;

// TODO: minimal map size

typedef int EntityId;

class Model {
public:
  Model() = delete;
  Model(Vec2 map_size, int n_snakes, int n_rabbits);
  ~Model() = default;

  ResponseCode Tick();

  EntityId GetPlayerSnakeId();
  Entity*  GetEntity(EntityId entity_id);

  Vec2 map_size_;

private:
  EntityId AddRabbit(Vec2 pos);

  ResponseCode RemoveEntity(EntityId entity_id);

  ResponseCode MoveSnake(EntityId snake_id);
  ResponseCode GrowSnake(EntityId snake_id);

  Vec2     GetRandomPosition();
  EntityId IsOccupied(Vec2 tile); // -1 if not occupied

  int                               n_rabbits_;
  int                               n_rabbits_max_;
  int                               n_snakes_;
  EntityId                          player_snake_;
  std::array<Entity*, MAX_ENTITIES> entities_;
};