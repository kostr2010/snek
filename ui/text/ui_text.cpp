#include "./ui_text.hpp"
#include "../../utils/assertm.hpp"
#include "../../utils/log.hpp"
#include "../../utils/vec2.hpp"

#include <cassert>
#include <csignal>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

TextUI::TextUI(Model* model, int tick_rate_ms) {
  LOG_LVL_VIEW_INIT();

  model_ = model;

  game_is_finished_ = false;
  game_is_paused_   = false;

  tick_rate_ms_ = tick_rate_ms;
}

void TextUI::Init(int n_snakes, int n_rabbits) {
  LOG_LVL_VIEW_ROUTINE("initializating text ui...");

  winsize win_sz = TextUI::GetScreenSize();

  LOG_LVL_VIEW_ROUTINE("actualized screen size: " << win_sz.ws_col << ", " << win_sz.ws_row);

  LOG_LVL_VIEW_ROUTINE("initializing model...");
  model_->Init({win_sz.ws_col, win_sz.ws_row}, n_snakes, n_rabbits);
  LOG_LVL_VIEW_ROUTINE("model initialized");

  AddBinding([](Model* model) { model->Tick(); }, 100);
  AddBinding([](Model* model) { TextUI::DrawScene(model); }, 100);

  LOG_LVL_VIEW_ROUTINE("drawing scene...");
  TextUI::DrawScene(model_);
  LOG_LVL_VIEW_ROUTINE("done");

  fflush(stdout);

  // ???

  struct termios sets;
  tcgetattr(0, &sets);

  sets.c_lflag &= ~ICANON;
  sets.c_lflag &= ~ECHO;

  tcsetattr(STDIN_FILENO, TCSANOW, &sets);

  //   signal(SIGTERM, TermHandler);
  //   signal(SIGINT, TermHandler);
  //   signal(SIGTSTP, TermHandler);

  LOG_LVL_VIEW_ROUTINE("text ui initialized");
}

// void TextUI::ActualizeScreenSize() {
//   ioctl(0, TIOCGWINSZ, &win_sz);
// }

winsize TextUI::GetScreenSize() {
  winsize win_sz;

  ioctl(0, TIOCGWINSZ, &win_sz);

  return win_sz;
}

void TextUI::RunModel() {
  int time_keeper = 0;
  // DrawScene(model_);

  while (!game_is_finished_) {
    // FIXME: screen tearing
    // DrawScene(model_);

    while (game_is_paused_)
      GetUserInput();

    struct pollfd poll_struct = {};
    poll_struct.fd            = STDIN_FILENO;
    poll_struct.events        = POLLIN;

    struct timespec start, finish;

    clock_gettime(CLOCK_REALTIME, &start);

    int fds_triggered = poll(&poll_struct, 1, tick_rate_ms_);

    clock_gettime(CLOCK_REALTIME, &finish);

    // in ms
    int passed_interval =
        (finish.tv_sec - start.tv_sec) * 1e3 + (finish.tv_nsec - start.tv_nsec) / 1e6 + 1;

    time_keeper += passed_interval;

    if (time_keeper >= tick_rate_ms_) {
      time_keeper -= tick_rate_ms_;

      for (auto& handler : time_bindings_)
        if (--handler.ms_left == 0) {
          handler.handler(model_);

          handler.ms_left = handler.period_ms;
        }
    }

    if (fds_triggered != 0)
      GetUserInput();

    // PainterScore(my_game);
    fflush(stdin);
  }

  FinishModel();
  //   ShowResults(my_game);
}

void TextUI::FinishModel() {
  game_is_finished_ = true;

  struct termios sets;
  tcgetattr(0, &sets);

  sets.c_lflag |= ICANON;
  sets.c_lflag |= ECHO;

  tcsetattr(STDIN_FILENO, TCSANOW, &sets);

  signal(SIGWINCH, SIG_DFL);
  signal(SIGTERM, SIG_DFL);
  signal(SIGINT, SIG_DFL);
  signal(SIGTSTP, SIG_DFL);
}

void TextUI::PauseModel() {
}

void TextUI::DrawScene(Model* model) {
  winsize win_sz = TextUI::GetScreenSize();

  TextUI::ClearScreen();
  TextUI::DrawGameBoundary();

  LOG_LVL_VIEW_ROUTINE("set model's map size from " << model->GetMapSize() << " to "
                                                    << win_sz.ws_col << ", " << win_sz.ws_row);
  model->SetMapSize({win_sz.ws_col, win_sz.ws_row});

  // ALL OF THE SUFFERINGS JUST FOR THE SAKE OF HTIS LINE BEING ABLE
  model->IterateRabbits(TextUI::DrawGameInstance, MAX_SNAKES, model->GetNRabbits() + MAX_SNAKES);

  model->IterateSnakes(TextUI::DrawGameInstance, 0, model->GetNSnakes());

  fflush(stdout);

  TextUI::SetCoursor({win_sz.ws_row + 1, 1});
}

void TextUI::AddBinding(KeyHandler handler) {
  assertm(handler != nullptr, "invalid ponter to KeyHandler function");

  key_bindings_.push_back(handler);
}

void TextUI::AddBinding(TickHandler handler, int period_ms) {
  assertm(handler != nullptr, "invalid ponter to TickHandler function");

  TimeHandler time_handler = {handler, period_ms, period_ms};
  time_bindings_.push_back(time_handler);
}

void TextUI::AddBinding(TimeHandler handler) {
  assertm(handler.handler != nullptr, "invalid ponter to TickHandler function");

  time_bindings_.push_back(handler);
}

void TextUI::SetCoursor(Vec2 pos) {
  std::printf("\e[%d;%dH", pos.x, pos.y);
}

int TextUI::TagToColor(int tag) {
  switch (tag) {
  case 0:
    return 31; // red
    break;
  case 1:
    return 32; // red
    break;
  case 2:
    return 33; // red
    break;
  case 3:
    return 34; // red
    break;
  }

  return -1;
}

void TextUI::GetUserInput() {
  char entered_symb = getchar();

  switch (entered_symb) {
  case 'p':
    game_is_paused_ = !game_is_paused_;
    break;
  case 27: // esc
    game_is_finished_ = true;
    break;
  }

  if (!game_is_paused_)
    for (const auto& handler : key_bindings_)
      handler(entered_symb, model_);
}

void TextUI::DrawGameInstance(Rabbit* rabbit) {
  LOG_LVL_VIEW_ROUTINE("drawing rabbit at " << rabbit->position_);

  printf("\e[1;%dm", 37);

  TextUI::DrawChar({rabbit->position_.x, rabbit->position_.y}, '@');

  printf("\e[0;%dm", 37);

  fflush(stdout);
}

void TextUI::DrawGameInstance(Snake* snake) {
  LOG_LVL_VIEW_ROUTINE("drawing snake " << snake->tag_ << " at " << snake->segments_.front());

  int color = TextUI::TagToColor(snake->tag_);

  printf("\e[1;%dm", color);

  // printf("%3u", snake->score_);

  char head = "AV<>"[snake->move_direction_];

  for (const auto& seg : snake->segments_) {
    if (seg == snake->segments_.front())
      TextUI::DrawChar(seg, head);
    else
      TextUI::DrawChar(seg, 'O');
  }

  printf("\e[1;%dm", 37);
  fflush(stdout);
}

void TextUI::DrawGameBoundary() {
  winsize win_sz = TextUI::GetScreenSize();

  //   #------>
  //   |     y
  //   |
  //   V x

  TextUI::DrawChar({1, 1}, '#');
  TextUI::DrawHorizontalLine({1, 2}, win_sz.ws_col - 2);
  TextUI::DrawChar({win_sz.ws_col, 1}, '#');

  TextUI::DrawChar({1, win_sz.ws_row}, '#');
  TextUI::DrawHorizontalLine({win_sz.ws_row, 2}, win_sz.ws_col - 2);
  TextUI::DrawChar({win_sz.ws_col, win_sz.ws_row}, '#');

  TextUI::DrawVerticalLine({2, 1}, win_sz.ws_row - 2);
  TextUI::DrawVerticalLine({2, win_sz.ws_col}, win_sz.ws_row - 2);
}

void TextUI::DrawGameScore() {
}

void TextUI::ClearScreen() {
  std::printf("\e[H\e[J");
}

void TextUI::DrawChar(Vec2 pos, const char ch) {
  std::printf("\e[%d;%dH%c", pos.y, pos.x, ch);
}

void TextUI::DrawVerticalLine(Vec2 pos, int length) {
  winsize win_sz = TextUI::GetScreenSize();

  assertm(pos.x > 0 && pos.x <= win_sz.ws_row && pos.y > 0 && pos.y <= win_sz.ws_col,
          "invalid position to draw");

  std::printf("\e[%d;%dH", pos.x, pos.y);

  std::string str = (pos.y == win_sz.ws_col) ? "" : "\e[1D";

  for (auto i = 0; i < length; i++)
    std::printf("#\e[1B%s", str.c_str());
}

void TextUI::DrawHorizontalLine(Vec2 pos, int length) {
  winsize win_sz = TextUI::GetScreenSize();

  assertm(pos.x > 0 && pos.x <= win_sz.ws_row && pos.y > 0 && pos.y <= win_sz.ws_col,
          "invalid position to draw");

  std::printf("\e[%d;%dH", pos.x, pos.y);

  for (auto i = pos.x; i < pos.x + length; i++)
    std::printf("#");
}