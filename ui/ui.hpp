#pragma once

#include "../model/model.hpp"
#include "../utils/response.hpp"
#include "../utils/vec2.hpp"

typedef void (*KeyHandler)(const char key, Model* model);
typedef void (*TickHandler)(Model* model);

struct TimeHandler {
  TickHandler handler;
  int         period_ms;
  int         ms_left;
};

class BaseUI {
public:
  BaseUI()  = default;
  ~BaseUI() = default;

  virtual void RunModel()    = 0;
  virtual void FinishModel() = 0;
  virtual void PauseModel()  = 0;

  virtual void AddBinding(KeyHandler handler)                 = 0;
  virtual void AddBinding(TickHandler handler, int period_ms) = 0;
  virtual void AddBinding(TimeHandler handler)                = 0;
};