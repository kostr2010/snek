#pragma once

#include "../../model/model.hpp"
#include "../ui.hpp"
#include <sys/ioctl.h>

#include <vector>

class TextUI : public BaseUI {
public:
  TextUI(Model* model, int tick_rate_ms);
  ~TextUI() = default;

  void Init(int n_snakes, int n_rabbits);

  // winsize ActualizeScreenSize();

  void RunModel() override;
  void FinishModel() override;
  void PauseModel() override;

  static void DrawScene(Model* model);

  void AddBinding(KeyHandler handler) override;
  void AddBinding(TickHandler handler, int period_ms) override;
  void AddBinding(TimeHandler handler) override;

private:
  static void SigHandler(int signal);

  void GetUserInput();

  static winsize GetScreenSize();

  static void DrawGameInstance(Rabbit* rabbit);
  static void DrawGameInstance(Snake* snake);

  static void DrawGameBoundary();
  static void DrawGameScore();

  static void ClearScreen();

  static void SetCoursor(Vec2 pos);

  static int TagToColor(int tag);

  static void DrawChar(Vec2 pos, const char ch);
  static void DrawVerticalLine(Vec2 pos, int length);
  static void DrawHorizontalLine(Vec2 pos, int length);

  std::vector<KeyHandler>  key_bindings_;
  std::vector<TimeHandler> time_bindings_;

  Model* model_;

  int  tick_rate_ms_;
  bool game_is_paused_;
  bool game_is_finished_;
};