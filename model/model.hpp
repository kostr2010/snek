#pragma once

// #include "../utils/direction.hpp"
#include "../entity/entity.hpp"
#include "../utils/log.hpp"
#include "../utils/response.hpp"
#include "../utils/vec2.hpp"

#include <array>
#include <set>
#include <vector>

const int MAX_RABBITS  = 20;
const int MAX_SNAKES   = 6;
const int MAX_ENTITIES = MAX_RABBITS + MAX_SNAKES;

const int MIN_MAP_X = 10;
const int MIN_MAP_Y = 10;

// TODO: minimal map size

typedef int EntityId;

class Model;

typedef void (*SnakeHandler1)(Snake* snake, Model* model);
typedef void (*SnakeHandler2)(Snake* snake);
typedef void (*RabbitHandler1)(Rabbit* rabbit, Model* model);
typedef void (*RabbitHandler2)(Rabbit* rabbit);

class Model {
public:
  Model();
  ~Model() = default;

  void Init(Vec2 map_size, int n_snakes, int n_rabbits);

  ResponseCode Tick();

  EntityId GetPlayerSnakeId();
  int      GetNSnakes();
  int      GetNRabbits();

  ResponseCode SetMapSize(Vec2 new_size);
  Vec2         GetMapSize();

  Vec2 GetNearestRabbit(Vec2 origin);

  void IterateSnakes(SnakeHandler1 handler, EntityId from = 0, EntityId to = MAX_SNAKES);
  void IterateSnakes(SnakeHandler2 handler, EntityId from = 0, EntityId to = MAX_SNAKES);
  void IterateRabbits(RabbitHandler1 handler, EntityId from = MAX_SNAKES,
                      EntityId to = MAX_SNAKES + MAX_RABBITS);
  void IterateRabbits(RabbitHandler2 handler, EntityId from = MAX_SNAKES,
                      EntityId to = MAX_SNAKES + MAX_RABBITS);

  EntityId IsOccupied(Vec2 tile); // -1 if not occupied
  bool     IsOutOfBounds(Vec2 tile);

private:
  Vec2 map_size_;

  EntityId AddRabbit(Vec2 pos);

  ResponseCode RemoveEntity(EntityId entity_id);

  ResponseCode MoveSnake(EntityId snake_id);

  Vec2 GetRandomPosition();

  int                               n_rabbits_;
  int                               n_rabbits_max_;
  int                               n_snakes_;
  EntityId                          player_snake_;
  std::array<Entity*, MAX_ENTITIES> entities_;
};