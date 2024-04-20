#include "review_game_console.hh"
#include "review_game_controller.hh"
#include "local_board_commander.hh"
#include "not_implemented_error.hh"

using std::string;


ReviewGameConsole::ReviewGameConsole(std::ostream &out, std::istream &in,
                            std::shared_ptr<LocalBoardReview> board,
                            std::shared_ptr<ReviewGameController> controller)
    : _out{out}, _in{in}, _board{std::move(board)}, _controller{std::move(controller)}{}

void ReviewGameConsole::displayGameReview(){
    system("clear");
    std::cout << "┌────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Review game: " << _board->getSessionId() << "\n";
    std::cout << "└────────────────────────────────────────────────────────────────────────────────┘\n";
    
}

ReturnInput ReviewGameConsole::handleInput(){
    return {ReturnInput::REVIEW_MENU, ""};
}

