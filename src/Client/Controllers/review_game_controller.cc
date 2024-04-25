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
    std::ifstream file("/home/manon/Desktop/pannee2/group-9/src/Client/Controllers/game_history.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }
    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    if (jsonData.contains("states")) {
        nlohmann::json membersArray = jsonData["states"];
        for (const auto& member : membersArray) {
            _move_list.push_back(member);
        }
    }
    //const std::string session = _board->getSessionId();
    //nlohmann::json jsonData =_client->GetGameHistory(session);
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

