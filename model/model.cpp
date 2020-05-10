#include "./model.hpp"
#include "../utils/assertm.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

Model::Model(Vec2 map_size, int n_snakes, int n_rabbits) {
  LOG_LVL_MODEL_INIT();

  assertm(n_snakes <= MAX_SNAKES && n_snakes > 0, "[MODEL] invalid snakes amount");
  assertm(n_rabbits <= MAX_RABBITS && n_rabbits > 0, "[MODEL] invalid rabbits amount");
  assertm(map_size.x >= MIN_MAP_X && map_size.y >= MIN_MAP_Y, "[MODEL] invalid map size");

  map_size_ = map_size;

  // spawning snakes
  int offset_x = map_size.x / (n_snakes + 1);
  int offset_y = map_size.y / 2;

  for (int i = 0; i < n_snakes; i++) {
    Vec2 head_i(ELEM_VECTOR_X * offset_x * (i + 1) + ELEM_VECTOR_Y * offset_y);
    std::cout << "> " << head_i << "\n";
    entities_[i] = new Snake({head_i});
  }

  for (int i = n_snakes; i < MAX_SNAKES; i++)
    entities_[i] = new Snake({{0, 0}});

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

  for (int i = 0; i < n_snakes_; i++) {
    if (MoveSnake(i) == ResponseCode::Error) {
      return ResponseCode::Error;
    }

    if (n_rabbits_ < n_rabbits_max_) {
      AddRabbit(GetRandomPosition());
    }
  }
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

Entity* Model::GetEntity(EntityId entity_id) {
  if (entity_id < 0 || entity_id >= n_rabbits_max_ + MAX_SNAKES) {
    LOG_LVL_MODEL_FAILURE("invalid entity_id " << entity_id << ". returning nullptr");

    return nullptr;
  }

  return entities_[entity_id];
}

EntityId Model::AddRabbit(Vec2 pos) {
  if (n_rabbits_ >= MAX_SNAKES + n_rabbits_max_) {
    LOG_LVL_MODEL_FAILURE("max amount of rabbits reached. unable to add another one");

    return -1;
  }

  ((Rabbit*)(entities_[MAX_SNAKES + n_rabbits_]))->position_ = pos;

  LOG_LVL_MODEL_ROUTINE("rabbit " << n_rabbits_ << " spawned at pos " << pos);

  return n_rabbits_++;
}

ResponseCode Model::RemoveEntity(EntityId entity_id) {
  if (entity_id < 0 || entity_id >= n_rabbits_max_ + MAX_SNAKES) {
    LOG_LVL_MODEL_FAILURE("invalid id " << entity_id << " given to remove");

    return ResponseCode::Error;
  } else if (entity_id <= n_snakes_ - 1) {
    if (entity_id != n_snakes_ - 1)
      entities_[entity_id] = entities_[n_snakes_ - 1];

    n_snakes_--;

    LOG_LVL_MODEL_ROUTINE("snake " << entity_id << " removed. now there are " << n_snakes_
                                   << " snakes on the map");

    return ResponseCode::Success;
  } else if (entity_id < MAX_SNAKES + n_rabbits_ - 1) {
    if (entity_id != MAX_SNAKES + n_rabbits_ - 1)
      entities_[entity_id] = entities_[MAX_SNAKES + n_rabbits_ - 1];

    n_rabbits_--;

    LOG_LVL_MODEL_ROUTINE("rabbit " << entity_id << " removed. now there are " << n_rabbits_
                                    << " rabbits on the map");

    return ResponseCode::Success;
  }

  LOG_LVL_MODEL_FAILURE("no active entity " << entity_id << " was found");
  return ResponseCode::Failure;
}

ResponseCode Model::MoveSnake(EntityId snake_id) {
  if (snake_id < 0 || snake_id >= n_snakes_) {
    LOG_LVL_MODEL_FAILURE("invalid snake_id " << snake_id << " given (inactive or out of range)");

    return ResponseCode::Error;
  }

  Vec2 new_head_pos    = ((Snake*)(entities_[snake_id]))->GetNewHeadPos();
  bool out_of_bounds_x = new_head_pos.x < 0 || new_head_pos.x > map_size_.x;
  bool out_of_bounds_y = new_head_pos.y < 0 || new_head_pos.y > map_size_.y;

  EntityId occupant = IsOccupied(new_head_pos);

  LOG_LVL_MODEL_ROUTINE("snake " << snake_id << " tries to move to " << new_head_pos
                                 << ", occupied by entity " << occupant);

  if (occupant == -1) {
    LOG_LVL_MODEL_ROUTINE("snake " << snake_id << " successfully moved to " << new_head_pos);

    ((Snake*)(entities_[snake_id]))->Move();

    return ResponseCode::Success;
  } else if (occupant < n_snakes_) {
    LOG_LVL_MODEL_FAILURE("snake " << snake_id << " died from collision with another snake "
                                   << occupant);

    RemoveEntity(snake_id);

    return ResponseCode::Failure;
  } else if (occupant >= MAX_SNAKES && occupant < MAX_SNAKES + n_rabbits_) {
    LOG_LVL_MODEL_ROUTINE("snake " << snake_id << "ate rabbit " << occupant);

    RemoveEntity(occupant);

    ((Snake*)(entities_[snake_id]))->Move();

    return ResponseCode::Success;
  } else if (out_of_bounds_x || out_of_bounds_y) {
    LOG_LVL_MODEL_FAILURE("snake " << snake_id << " died from collision boundary");

    RemoveEntity(snake_id);

    return ResponseCode::Failure;
  } else {
    LOG_LVL_VIEW_FAILURE("invalid occupant id " << occupant << " recieved");

    return ResponseCode::Error;
  }
}

ResponseCode Model::GrowSnake(EntityId snake_id) {
  if (snake_id < 0 || snake_id >= n_snakes_) {
    LOG_LVL_MODEL_FAILURE("invalid snake_id " << snake_id << " given (inactive or out of range)");

    return ResponseCode::Error;
  }

  ((Snake*)(entities_[snake_id]))->Grow();

  return ResponseCode::Success;
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
