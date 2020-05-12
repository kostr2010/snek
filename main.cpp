#include "./controller/controller.hpp"
#include "./model/model.hpp"
#include "./ui/text/ui_text.hpp"

int main() {
  Model model{};

  ControllerPlayer   controller_player{};
  ControllerComputer controller_computer{};

  TextUI ui(&model, 1);

  ui.AddBinding(controller_player.GetKeyHandler());
  ui.AddBinding(controller_computer.GetTickHandler(), 100);

  ui.Init(4, 10);

  ui.RunModel();

  return 0;
}