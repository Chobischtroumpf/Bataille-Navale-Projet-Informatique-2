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
        return handleFire(player, move);
    }else if (str_move == "placeShips"){
        return handlePlaceShip(player, move);
    }else{
        //handle error
        return false;
    }
}

bool GameState::handleFire(PlayerRole player, const nlohmann::json& move){
    int ability = move["fire"]["ability"];
    size_t x = move["fire"]["anchor"][0];
    size_t y = move["fire"]["anchor"][1];
    BoardCoordinates board_coordinates{x,y};
    return game->handleFire(role_to_turn(player),board_coordinates);

}

bool GameState::handlePlaceShip(PlayerRole player, const nlohmann::json& move){
    /*if (move.size() != 5){
        //handle the number of placement is incorrect
        return false;
    }*/

    for (const auto& obj_ship : move["ships"]) {

        size_t x = obj_ship["anchor"][0];
        size_t y = obj_ship["anchor"][1];

        BoardCoordinates top_left{x,y};

        std::vector<BoardCoordinates> coords{};
        for (const auto& obj_coord : obj_ship["coordinates"]) {
            size_t i = obj_coord[0];
            size_t j = obj_coord[1];
            BoardCoordinates board_coordinates{i,j};
            coords.push_back(board_coordinates);
        }
        Ship ship{top_left, coords};
        ship.setType(obj_ship["type"]);

        bool result = game->handlePlaceShip(role_to_turn(player),ship);
        if (!result){
            //error in placing ship gotta be handeled
            return false;
        }        
    }
    return true;
}

Turn GameState::role_to_turn(PlayerRole player) {
    if (player == PlayerRole::Leader) {
        return Turn::PLAYERONE;
    } else if (player == PlayerRole::Opponent) {
        return Turn::PLAYERTWO;
    } else {
        // Spectator has no turn
        // You might handle this case differently based on your requirements
        // For example, you can throw an exception or return a default turn
        // Here, I'll just return PLAYERONE as default
        return Turn::PLAYERONE;
    }
}


nlohmann::json GameState::getGameState(PlayerRole player) const {
    // Return a placeholder or current game state
    // For simplicity, we return currentState directly
    return game->getState(player);
}
