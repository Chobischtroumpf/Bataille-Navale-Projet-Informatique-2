#include "review_game_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"
#include "local_board_commander.hh"


ReviewGameController::ReviewGameController(std::shared_ptr<GameClient> client, std::shared_ptr<LocalBoardReview> board)
    : _client{client},_board{std::move(board)} {
        std::cout << "controller game review constr." << std::endl;
        _current_move = 0;
        requestMovesList();
        initLocalBoard();
    }

void ReviewGameController::requestMovesList(){
    const std::string session = _board->getSessionId();
    std::future<nlohmann::json> future_json = _client->GetGameHistory(session);
    nlohmann::json json_data = future_json.get();
    std::clog << "BBBBBBBBBBB  " << json_data << std::endl;

    if (json_data.contains("states")) {
        nlohmann::json membersArray = json_data["states"];
        for (const auto& member : membersArray) {
            std::clog << "AAAAAAAAAAAAAAAA  " << member << std::endl;
            _move_list.push_back(member);
        }
    }
}

void ReviewGameController::initLocalBoard(){
    std::clog << "!!init!!!  " << _move_list[0] << std::endl;
    _board->update_board(_move_list[0]);
}

void ReviewGameController::setNextMove(){
    if (_current_move < _move_list.size()-1){
        std::clog << "!!next!!!  " << _move_list[_current_move] << std::endl;
        _current_move += 1;
        _board->update_board(_move_list[_current_move]);
    }
}

void ReviewGameController::setPreviousMove(){
    if (_current_move >= 1){
        std::clog << "!!previous!!!  " << _move_list[0] << std::endl;
        _current_move -= 1;
        _board->update_board(_move_list[_current_move]);
    }
}

