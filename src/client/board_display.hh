#pragma once

/** Interface for Board Display */
class BoardDisplay {
 protected:
  BoardDisplay(const BoardDisplay&)            = default;
  BoardDisplay(BoardDisplay&&)                 = default;
  BoardDisplay& operator=(const BoardDisplay&) = default;
  BoardDisplay& operator=(BoardDisplay&&)      = default;

 public:
  BoardDisplay() = default;

  /** To call when the display is to be updated. Can be called after an
   * event or at regular intervals. */
  virtual void update()      = 0;
  /** To call when the BoardControl is ready to receive an input from the view. The exact
   * meaning have to be precised in the concrete classes. */
  virtual void handleInput() = 0;

  // Make destructor virtual
  virtual ~BoardDisplay() = default;
};
