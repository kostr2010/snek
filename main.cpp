#include "./utils/log.hpp"

int main() {
  LOG_LVL_CONTROLLER_INIT();
  LOG_LVL_MODEL_INIT();
  LOG_LVL_VIEW_INIT();

  return 0;
}