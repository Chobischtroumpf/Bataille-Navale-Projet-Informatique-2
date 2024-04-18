#include "ship.hh"
#include "not_implemented_error.hh"
#include <optional>
#include <string>
#include <vector>

class ShipClassic {
  protected:
    std::vector<Ship> _ships;
    int _pos = 0;
  public:
    ShipClassic(int number_of_case);
    void next();
    void previous();
    Ship getShip();
    void rotate();

    virtual ~ShipClassic() = default;
};