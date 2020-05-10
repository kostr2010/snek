#include "./model.hpp"

/*
class Entity {
public:
  Entity();
  Entity(Vec2 pos, bool active = false);

  ~Entity() = default;

  Vec2 pos_{};

  bool active_;

private:
};

typedef Entity Rabbit;

class Snake : public Entity {
public:
  Snake();
  ~Snake() = default;

private:
  Direction         move_direction_ = Direction::Neutral;
  std::vector<Vec2> segments_{};
};

typedef int SnakeId;
typedef int RabbitId;

class Model {
public:
  Model(Vec2 map_size, int n_snakes, int n_rabbits);
  ~Model() = default;

  void Tick();

  ResponseCode SetSnakeMoveDirection(SnakeId snake);
  Direction    GetSnakeMoveDirection(SnakeId snake);

  std::vector<SnakeId> GetSnakeViableIds();
  SnakeId              GetPlayerSnakeId();

private:
  SnakeId      AddSnake(Vec2 pos);
  ResponseCode RemoveSnake(SnakeId id);

  RabbitId     AddRabbit(Vec2 pos);
  ResponseCode RemoveRabbit(RabbitId id);

  ResponseCode MoveSnake(SnakeId id);
  void         GrowSnake(SnakeId id);

  Vec2                            map_size;
  std::vector<Vec2>               occupied_tiles_;
  int                             n_snakes_;
  SnakeId                         player_snake_;
  std::array<Snake, MAX_SNAKES>   snakes_;
  int                             n_rabbits;
  std::array<Rabbit, MAX_RABBITS> rabbits_;
};
*/