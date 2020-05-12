#pragma once

#include "../model/model.hpp"
#include "../ui/ui.hpp"

class Controller {
public:
  virtual KeyHandler  GetKeyHandler()  = 0;
  virtual TickHandler GetTickHandler() = 0;
};

class ControllerPlayer : public Controller {
public:
  ControllerPlayer();
  ~ControllerPlayer() = default;

  KeyHandler  GetKeyHandler() override;
  TickHandler GetTickHandler() override;

private:
  KeyHandler handler_;
};

class ControllerComputer : public Controller {
public:
  ControllerComputer();
  ~ControllerComputer() = default;

  KeyHandler  GetKeyHandler() override;
  TickHandler GetTickHandler() override;

private:
  TickHandler handler_;
};