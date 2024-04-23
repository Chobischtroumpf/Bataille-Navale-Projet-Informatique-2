#include "local_board_review.hh"

LocalBoardReview::LocalBoardReview(const std::string &session_id)
    : _session_id{session_id},
      _my_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))},
      _their_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))}
      {}


void LocalBoardReview::update_board(const nlohmann::json &new_board) {
    std::vector<std::string> fleet_names = {"fleetA", "fleetB"};
    for(auto fname: fleet_names){
        auto fleet = new_board[fname];
        auto& board = (fname == "fleetA") ? _my_board : _their_board;
        if (!(fleet.is_string() && fleet.get<std::string>() == "None")) {
            for (int i = 0; i < height(); i++) {
                for (int j = 0; j < width(); j++) {
                    board[i][j].setType(string_to_celltype(fleet[i][j]["type"]));
                }
            }
        }
    }
}


CellType LocalBoardReview::string_to_celltype(const std::string &type) {
  if (type == "WATER") {
    return WATER;
  } else if (type == "OCEAN") {
    return OCEAN;
  } else if (type == "UNDAMAGED_MINE") {
    return UNDAMAGED_MINE;
  } else if (type == "SCANNED_MINE") {
    return SCANNED_MINE;
  } else if (type == "HIT_MINE") {
    return HIT_MINE;
  } else if (type == "UNDAMAGED_SHIP") {
    return UNDAMAGED_SHIP;
  } else if (type == "SCANNED_SHIP") {
    return SCANNED_SHIP;
  } else if (type == "HIT_SHIP") {
    return HIT_SHIP;
  } else if (type == "SUNK_SHIP") {
    return SUNK_SHIP;
  } else {
    throw std::runtime_error("Unknown CellType: " + type);
  }
}


std::size_t LocalBoardReview::width() const {
    return _my_board.at(0).size();
}


std::size_t LocalBoardReview::height() const { 
    return _my_board.size(); 
}

const std::string LocalBoardReview::getSessionId(){
    return _session_id;
}

CellType LocalBoardReview::best(CellType lhs, CellType rhs) {
  if (!(lhs & UNDAMAGED_SHIP) || !(rhs & UNDAMAGED_SHIP)) {
    std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
              << static_cast<unsigned>(rhs) << ")" << std::endl;
    throw std::logic_error("BoardView::best called with non-ship types");
  }
  return lhs <= rhs ? lhs : rhs;
}