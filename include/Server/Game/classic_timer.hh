#include "game_timer.hh"

class ClassicTimer : public GameTimer {
private:
  void gameTimeRunout();

public:
  ClassicTimer();

  void set(int game_time, int player_time,
           std::function<void()> callback_function = {}) override;
  void turnReset() override;
  void switchTurn() override;
};
