#include "game.hpp"

int main(void) {

  std::srand(static_cast<unsigned>(time(NULL)));

  Game game;

  while (game.running()) {

    auto frame_start = std::chrono::high_resolution_clock::now(); // FPS
                                                                  // tracking

    game.update();

    game.render(); // Rendering

    auto frame_end = std::chrono::high_resolution_clock::now();
    auto frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        frame_end - frame_start);
    float current_fps = 1e9f / frame_time.count();

    game.setFPS(current_fps);
  }

  return 0;
}
