#include "review_game_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"
#include "local_board_commander.hh"


ReviewGameController::ReviewGameController(std::shared_ptr<GameClient> client, std::shared_ptr<LocalBoardReview> board)
    : _client{client},_board{std::move(board)} {
        std::cout << "controller game review constr." << std::endl;
        _current_move = 0;
        requestMovesList();
        std::cout << "1." << std::endl;
        //initLocalBoard();
        std::cout << "2." << std::endl;

    }

void ReviewGameController::requestMovesList(){
    const std::string session = _board->getSessionId();
    //_move_list = _client->GetGameHistory(session);
}

void ReviewGameController::initLocalBoard(){
    _board->update_board(_move_list[0]);
}

void ReviewGameController::setNextMove(){
    if (_current_move <= _move_list.size()-2){
        _current_move += 1;
        _board->update_board(_move_list[_current_move]);
    }
}

void ReviewGameController::setPreviousMove(){
    if (_current_move >= 1){
        _current_move -= 1;
        _board->update_board(_move_list[_current_move]);
    }
}

