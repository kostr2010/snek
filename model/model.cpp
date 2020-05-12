#include "./model.hpp"
#include "../utils/assertm.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

Model::Model() {
  LOG_LVL_MODEL_INIT();
}

void Model::Init(Vec2 map_size, int n_snakes, int n_rabbits) {
  assertm(n_snakes <= MAX_SNAKES && n_snakes > 0, "[MODEL] invalid snakes amount");
  assertm(n_rabbits <= MAX_RABBITS && n_rabbits > 0, "[MODEL] invalid rabbits amount");
  assertm(map_size.x >= MIN_MAP_X && map_size.y >= MIN_MAP_Y, "[MODEL] invalid map size");

  map_size_ = map_size;

  // spawning snakes
  int offset_x = map_size.x / (n_snakes + 1);
  int offset_y = map_size.y / 2;

  for (int i = 0; i < n_snakes; i++) {
    Vec2 head_i(ELEM_VECTOR_X * offset_x * (i + 1) + ELEM_VECTOR_Y * offset_y);
    entities_[i] = new Snake({head_i}, i);
  }

  for (int i = n_snakes; i < MAX_SNAKES; i++)
    entities_[i] = new Snake({{0, 0}}, i);

  n_snakes_ = n_snakes;

  LOG_LVL_MODEL_ROUTINE("initial snakes spawned");

  // spawning rabbits
  for (int i = MAX_SNAKES; i < MAX_ENTITIES; i++) {
    entities_[i] = new Rabbit({0, 0});
  }

  n_rabbits_max_ = n_rabbits;
  n_rabbits_     = 0;

  for (int i = 0; i < n_rabbits_max_; i++) {
    AddRabbit(GetRandomPosition());
    for (int i = 0; i < 10000; i++)
      ;
  }

  LOG_LVL_MODEL_ROUTINE("initial rabbits spawned");

  player_snake_ = 0;

  LOG_LVL_MODEL_ROUTINE("model initialized");
}

ResponseCode Model::Tick() {
  static int tick = 0;
  LOG_LVL_MODEL_ROUTINE("\n==================== TICK " << std::setw(4) << tick++
                                                       << "====================\n");

  for (int i = n_snakes_ - 1; i >= 0; i--)
    if (MoveSnake(i) == ResponseCode::Error)
      return ResponseCode::Error;

  return ResponseCode::Success;
}

Vec2 Model::GetRandomPosition() {
  std::srand(unsigned(std::time(0)));

  int  rand_min   = 5;
  int  rand_max_x = map_size_.x - 5;
  int  rand_max_y = map_size_.y - 5;
  Vec2 tile{};

  // TODO: add check for entirely busy map (f.ex counter of tries to find free tile)

  do {
    int rand_x = rand_min + (rand() % (rand_max_x - rand_min + 1));
    int rand_y = rand_min + (rand() % (rand_max_y - rand_min + 1));

    tile = {rand_x, rand_y};

  } while (IsOccupied(tile) != -1);

  return tile;
}

EntityId Model::GetPlayerSnakeId() {
  return player_snake_;
}

int Model::GetNSnakes() {
  return n_snakes_;
}

int Model::GetNRabbits() {
  return n_rabbits_;
}

ResponseCode Model::SetMapSize(Vec2 new_size) {
  assertm(new_size.x > MIN_MAP_X && new_size.y > MIN_MAP_Y, "invalid map resize");

  // FIXME: add dynamic map resize

  return ResponseCode::Success;
}

Vec2 Model::GetMapSize() {
  return map_size_;
}

Vec2 Model::GetNearestRabbit(Vec2 origin) {
  int  distance_min = map_size_.Length(); // each distance on the map is lesser than it's diagonal
  Vec2 nearest_pos  = map_size_;

  for (int i = MAX_SNAKES; i < MAX_SNAKES + n_rabbits_; i++) {
    Vec2 rabbit_pos = ((Rabbit*)entities_[i])->position_;

    Vec2 delta        = rabbit_pos - origin;
    int  distance_cur = delta.Length();

    if (distance_cur < distance_min) {
      distance_min = distance_cur;
      nearest_pos  = rabbit_pos;
    }
  }

  return nearest_pos;
}

void Model::IterateSnakes(SnakeHandler1 handler, EntityId from, EntityId to) {
  LOG_LVL_MODEL_ROUTINE("iterating snakes from " << from << " to " << to);

  for (int i = from; i < to; i++)
    handler((Snake*)entities_[i], this);

  LOG_LVL_MODEL_ROUTINE("done");
}

void Model::IterateSnakes(SnakeHandler2 handler, EntityId from, EntityId to) {
  LOG_LVL_MODEL_ROUTINE("iterating snakes from " << from << " to " << to);

  for (int i = from; i < to; i++)
    handler((Snake*)entities_[i]);

  LOG_LVL_MODEL_ROUTINE("done");
}

void Model::IterateRabbits(RabbitHandler1 handler, EntityId from, EntityId to) {
  LOG_LVL_MODEL_ROUTINE("iterating rabbits from " << from << " to " << to);

  for (int i = from; i < to; i++)
    handler((Rabbit*)entities_[i], this);

  LOG_LVL_MODEL_ROUTINE("done");
}

void Model::IterateRabbits(RabbitHandler2 handler, EntityId from, EntityId to) {
  LOG_LVL_MODEL_ROUTINE("iterating rabbits from " << from << " to " << to);

  for (int i = from; i < to; i++)
    handler((Rabbit*)entities_[i]);

  LOG_LVL_MODEL_ROUTINE("done");
}

EntityId Model::AddRabbit(Vec2 pos) {
  if (n_rabbits_ >= MAX_SNAKES + n_rabbits_max_) {
    LOG_LVL_MODEL_FAILURE("max amount of rabbits reached. unable to add another one");

    return -1;
  }

  ((Rabbit*)(entities_[MAX_SNAKES + n_rabbits_]))->position_ = pos;

  LOG_LVL_MODEL_ROUTINE("rabbit " << MAX_SNAKES + n_rabbits_ << " spawned at pos " << pos
                                  << "now there are " << n_rabbits_ + 1 << "rabbits on the map");
  IterateRabbits([](Rabbit* rabbit) { LOG_LVL_MODEL_ROUTINE("  > " << rabbit->position_); },
                 MAX_SNAKES,
                 MAX_SNAKES + n_rabbits_ + 1);

  return n_rabbits_++;
}

ResponseCode Model::RemoveEntity(EntityId entity_id) {
  if (entity_id < 0 || entity_id >= n_rabbits_max_ + MAX_SNAKES) {
    LOG_LVL_MODEL_FAILURE("invalid id " << entity_id << " given to remove");

    return ResponseCode::Error;
  } else if (entity_id < n_snakes_) {
    if (entity_id != n_snakes_ - 1)
      entities_[entity_id] = entities_[n_snakes_ - 1];

    if (entity_id == player_snake_) {
      LOG_LVL_MODEL_ROUTINE("PLAYER_DIED");
      player_snake_ = -1;
    }

    entities_[n_snakes_ - 1] = nullptr;

    n_snakes_--;

    LOG_LVL_MODEL_ROUTINE("snake " << entity_id << " removed. now there are " << n_snakes_
                                   << " snakes on the map");

    return ResponseCode::Success;
  } else if (entity_id < MAX_SNAKES + n_rabbits_ && entity_id >= MAX_SNAKES) {
    if (entity_id != MAX_SNAKES + n_rabbits_ - 1)
      ((Rabbit*)entities_[entity_id])->position_ =
          ((Rabbit*)entities_[MAX_SNAKES + n_rabbits_ - 1])->position_;

    n_rabbits_--;

    LOG_LVL_MODEL_ROUTINE("rabbit " << entity_id << " removed. now there are " << n_rabbits_
                                    << " rabbits on the map");

    IterateRabbits([](Rabbit* rabbit) { LOG_LVL_MODEL_ROUTINE("  > " << rabbit->position_); },
                   MAX_SNAKES,
                   MAX_SNAKES + n_rabbits_);

    return ResponseCode::Success;
  }

  LOG_LVL_MODEL_FAILURE("no active entity " << entity_id << " was found among " << n_snakes_
                                            << " snakes and " << n_rabbits_ << " rabbits");
  return ResponseCode::Failure;
}

ResponseCode Model::MoveSnake(EntityId snake_id) {
  if (snake_id < 0 || snake_id >= n_snakes_) {
    LOG_LVL_MODEL_FAILURE("invalid snake_id " << snake_id << " given (inactive or out of range)");

    return ResponseCode::Error;
  }

  Vec2 new_head_pos = ((Snake*)(entities_[snake_id]))->GetNewHeadPos();

  bool out_of_bounds = IsOutOfBounds(new_head_pos);

  EntityId occupant = IsOccupied(new_head_pos);

  LOG_LVL_MODEL_ROUTINE("snake " << snake_id << " tries to move to " << new_head_pos
                                 << ", occupied by entity " << occupant);

  if (out_of_bounds) {
    LOG_LVL_MODEL_FAILURE("snake " << snake_id << " died from collision boundary");

    RemoveEntity(snake_id);

    return ResponseCode::Failure;
  } else if (occupant < n_snakes_ && occupant >= 0) {
    LOG_LVL_MODEL_FAILURE("snake " << snake_id << " died from collision with another snake "
                                   << occupant);

    RemoveEntity(snake_id);

    return ResponseCode::Failure;
  } else if (occupant == -1) {
    LOG_LVL_MODEL_ROUTINE("snake " << snake_id << " successfully moved to " << new_head_pos);

    ((Snake*)(entities_[snake_id]))->Move();

    return ResponseCode::Success;
  } else if (occupant >= MAX_SNAKES && occupant < MAX_SNAKES + n_rabbits_) {
    LOG_LVL_MODEL_ROUTINE("snake " << snake_id << " ate rabbit " << occupant);

    RemoveEntity(occupant);
    AddRabbit(GetRandomPosition());

    ((Snake*)(entities_[snake_id]))->score_++;
    ((Snake*)(entities_[snake_id]))->Grow();
    ((Snake*)(entities_[snake_id]))->Move();

    return ResponseCode::Success;
  } else {
    LOG_LVL_VIEW_FAILURE("invalid occupant id " << occupant << " recieved");

    return ResponseCode::Error;
  }
}

EntityId Model::IsOccupied(Vec2 tile) {
  for (int i = 0; i < n_snakes_; i++) {
    if (entities_[i]->Occupies(tile))
      return i;
  }

  for (int i = MAX_SNAKES; i < MAX_SNAKES + n_rabbits_; i++) {
    if (entities_[i]->Occupies(tile))
      return i;
  }

  return -1;
}

bool Model::IsOutOfBounds(Vec2 tile) {
  return tile.x < 0 || tile.x >= map_size_.x || tile.y < 0 || tile.y >= map_size_.y;
}
