#include "../controller/controller.hpp"

ControllerPlayer::ControllerPlayer() {
  LOG_LVL_CONTROLLER_INIT();

  handler_ = [](const char key, Model* model) {
    EntityId player_snake_id = model->GetPlayerSnakeId();

    switch (key) {
    case 'a':
      model->IterateSnakes(
          [](Snake* snek, Model*) {
            if (snek->move_direction_ != Direction::Right)
              snek->move_direction_ = Direction::Left;
          },
          player_snake_id,
          player_snake_id + 1);
      break;
    case 's':
      model->IterateSnakes(
          [](Snake* snek, Model*) {
            if (snek->move_direction_ != Direction::Up)
              snek->move_direction_ = Direction::Down;
          },
          player_snake_id,
          player_snake_id + 1);
      break;
    case 'w':
      model->IterateSnakes(
          [](Snake* snek, Model*) {
            if (snek->move_direction_ != Direction::Down)
              snek->move_direction_ = Direction::Up;
          },
          player_snake_id,
          player_snake_id + 1);
      break;
    case 'd':
      model->IterateSnakes(
          [](Snake* snek, Model*) {
            if (snek->move_direction_ != Direction::Left)
              snek->move_direction_ = Direction::Right;
          },
          player_snake_id,
          player_snake_id + 1);
      break;
    default:
      break;
    }
  };
}

KeyHandler ControllerPlayer::GetKeyHandler() {
  return handler_;
}

TickHandler ControllerPlayer::GetTickHandler() {
  return nullptr;
}

ControllerComputer::ControllerComputer() {
  handler_ = [](Model* model) {
    model->IterateSnakes(
        [](Snake* snake, Model* model) {
          Vec2 snake_head = snake->segments_.front();

          Vec2 tile_up    = snake_head - ELEM_VECTOR_Y;
          Vec2 tile_down  = snake_head + ELEM_VECTOR_Y;
          Vec2 tile_left  = snake_head - ELEM_VECTOR_X;
          Vec2 tile_right = snake_head + ELEM_VECTOR_X;

          int entity_id_up    = model->IsOccupied(tile_up);
          int entity_id_down  = model->IsOccupied(tile_down);
          int entity_id_left  = model->IsOccupied(tile_left);
          int entity_id_right = model->IsOccupied(tile_right);

          bool is_out_of_bouns_up    = model->IsOutOfBounds(tile_up);
          bool is_out_of_bouns_down  = model->IsOutOfBounds(tile_down);
          bool is_out_of_bouns_left  = model->IsOutOfBounds(tile_left);
          bool is_out_of_bouns_right = model->IsOutOfBounds(tile_right);

          bool is_free_up    = entity_id_up == -1 && !is_out_of_bouns_up;
          bool is_free_down  = entity_id_down == -1 && !is_out_of_bouns_down;
          bool is_free_left  = entity_id_left == -1 && !is_out_of_bouns_left;
          bool is_free_right = entity_id_right == -1 && !is_out_of_bouns_right;

          Direction direction_current = snake->move_direction_;

          bool possible_directions[] = {
              (is_free_up || entity_id_up >= MAX_SNAKES) && direction_current != Direction::Down,
              (is_free_down || entity_id_down >= MAX_SNAKES) && direction_current != Direction::Up,
              (is_free_left || entity_id_left >= MAX_SNAKES) &&
                  direction_current != Direction::Right,
              (is_free_right || entity_id_right >= MAX_SNAKES) &&
                  direction_current != Direction::Left};

          Vec2 nearest_rabbit = model->GetNearestRabbit(snake->segments_.front());

          LOG_LVL_MODEL_ROUTINE("nearest rabbit for snake "
                                << snake->tag_ << " at " << snake->segments_ << " is at "
                                << nearest_rabbit << " free {up, down, left, right} "
                                << possible_directions[0] << possible_directions[1]
                                << possible_directions[2] << possible_directions[3]);

          // nearest rabbit for snake 2 at {{9, 5}, {10, 5}, } is at {155, 5} free 1110
          int dx = nearest_rabbit.x - snake_head.x;
          int dy = nearest_rabbit.y - snake_head.y;

          if (abs(dx) > abs(dy)) {
            if (dx > 0 && possible_directions[Direction::Right])
              snake->move_direction_ = Direction::Right;
            else if (dx < 0 && possible_directions[Direction::Left])
              snake->move_direction_ = Direction::Left;
            else if (possible_directions[Direction::Up])
              snake->move_direction_ = Direction::Up;
            else if (possible_directions[Direction::Down])
              snake->move_direction_ = Direction::Down;
          } else {
            if (dy > 0 && possible_directions[Direction::Down])
              snake->move_direction_ = Direction::Down;
            else if (dy < 0 && possible_directions[Direction::Up])
              snake->move_direction_ = Direction::Up;
            else if (possible_directions[Direction::Left])
              snake->move_direction_ = Direction::Left;
            else if (possible_directions[Direction::Right])
              snake->move_direction_ = Direction::Right;
          }

          // Direction up_or_down =
          //     (nearest_rabbit.y < snake_head.y) ? Direction::Up : Direction::Down;
          // Direction left_or_right =
          //     (nearest_rabbit.x < snake_head.x) ? Direction::Left : Direction::Right;
          // Direction snake_move_direction = snake->move_direction_;

          // if (possible_directions[snake_move_direction] &&
          //     (snake_move_direction == up_or_down || snake_move_direction == left_or_right))
          //   return;

          // if (possible_directions[up_or_down])
          //   snake->move_direction_ = up_or_down;
          // else if (possible_directions[left_or_right])
          //   snake->move_direction_ = left_or_right;
          // else if (possible_directions[0])
          //   snake->move_direction_ = Direction::Up;
          // else if (possible_directions[1])
          //   snake->move_direction_ = Direction::Down;
          // else if (possible_directions[2])
          //   snake->move_direction_ = Direction::Left;
          // else
          //   snake->move_direction_ = Direction::Right;
        },
        model->GetPlayerSnakeId() + 1,
        model->GetNSnakes());
  };
}

KeyHandler ControllerComputer::GetKeyHandler() {
  return nullptr;
}

TickHandler ControllerComputer::GetTickHandler() {
  return handler_;
}