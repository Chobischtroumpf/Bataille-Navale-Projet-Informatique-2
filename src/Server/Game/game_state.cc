#include "game_state.hh"

GameState::GameState(const nlohmann::json& gameDetails) : game{std::make_shared<Game>(gameDetails)} {
    /*// Initialize currentState with some default value or based on gameDetails
    currentState = nlohmann::json::object();*/
}

GameState::~GameState() {
    // Cleanup resources if needed
}

bool GameState::makeMove(PlayerRole player, const nlohmann::json& move) {
    std::string str_move = move.at("move").get<std::string>() ;
    if (str_move == "fire"){
        return handle_fire(player, move);
    }else if (str_move == "placeship"){
        return handle_place_ship(player, move);
    }else{
        //handle error
        return false;
    }
}

bool GameState::handle_fire(PlayerRole player, const nlohmann::json& move){
    std::string x = move["anchor"]["x"].get<std::string>();
        size_t y = move["anchor"]["y"];
        BoardCoordinates board_coordinates{BoardCoordinates::parseX(x).value(),y-1};
    if (player==Leader){        
        return game->handle_fire(PLAYERONE,board_coordinates);
    }else{
        return game->handle_fire(PLAYERTWO,board_coordinates);
    }

}

bool GameState::handle_place_ship(PlayerRole player, const nlohmann::json& move){
    
}



nlohmann::json GameState::getGameState(PlayerRole player) const {
    // Return a placeholder or current game state
    // For simplicity, we return currentState directly
    return game->get_state(player);
}
