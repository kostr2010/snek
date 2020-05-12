#include "./controller/controller.hpp"
#include "./model/model.hpp"
#include "./ui/text/ui_text.hpp"

int main() {
  Model model{};

  ControllerPlayer   controller_player{};
  ControllerComputer controller_computer{};

  TextUI ui(&model, 100);

  ui.AddBinding(controller_player.GetKeyHandler());
  ui.AddBinding(controller_computer.GetTickHandler(), 1);

  ui.Init(6, 15);

  ui.RunModel();

  return 0;
}