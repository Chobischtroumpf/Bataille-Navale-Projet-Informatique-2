#include "game_timer.hh"

class PendulumTimer : public GameTimer {
public:
  PendulumTimer();

  void set(int game_time, int player_time,
           std::function<void()> callback_function) override;
  void turnReset() override;
  void switchTurn() override;
};
