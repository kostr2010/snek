#include "./model/model.hpp"

int main() {
  Model model({40, 40}, 2, 2);

  for (int i = 0; i < 10; i++) {
    model.Tick();
  }

  return 0;
}