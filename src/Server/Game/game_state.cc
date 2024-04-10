#include "game_state.hh"
#include <iostream>

GameState::GameState(const nlohmann::json& gameDetails) : _game{std::make_shared<Game>(gameDetails)} {
    /*// Initialize currentState with some default value or based on gameDetails
    currentState = nlohmann::json::object();*/
}

GameState::~GameState() {
    // Cleanup resources if needed
}

bool GameState::makeMove(PlayerRole player, const nlohmann::json& move) {
    std::string str_move = move.at("moveType").get<std::string>() ;

    std::clog << "move type: " << str_move << std::endl;

    if (str_move == "fire") {
        std::clog << "fire move" << std::endl;
        return handleFire(player, move.at("fire"));
    } else if (str_move == "placeShips") {
        return handlePlaceShip(player, move.at("ships"));
    } else if (str_move == "chooseFaction") {
        //handle faction choice
        return true;
    }else{
        //handle error
        return false;
    }
}

bool GameState::handleFire(PlayerRole player, const nlohmann::json& fire_move){
    SpecialAbilityType ability_type = fire_move.at("ability").get<SpecialAbilityType>();
    size_t x = fire_move.at("anchor").at(0).get<size_t>();
    size_t y = fire_move.at("anchor").at(1).get<size_t>();
    BoardCoordinates board_coordinates{x,y};
    return _game->handleFire(role_to_turn(player), ability_type, board_coordinates);

}

bool GameState::handlePlaceShip(PlayerRole player, const nlohmann::json& ships){
    /*if (move.size() != 5){
        //handle the number of placement is incorrect
        return false;
    }*/

    for (const auto& obj_ship : ships) {

        std::clog << "ship: " << obj_ship << std::endl;
        size_t top_left_x = obj_ship.at("anchor").at("x");
        size_t top_left_y = obj_ship.at("anchor").at("y");

        std::clog << "create BoardCoordinates at: " << top_left_x << " " << top_left_y << std::endl;
        BoardCoordinates top_left{top_left_x, top_left_y};

        std::clog << "create vector of BoardCoordinates" << std::endl;
        std::vector<BoardCoordinates> coords{};
        const auto arr_coordinates = obj_ship.at("coordinates");
        std::clog << "arr_coordinates: " << arr_coordinates << std::endl;
        for (const auto& elem : arr_coordinates) {
            size_t i = elem.at("x");
            size_t j = elem.at("y");
            BoardCoordinates board_coordinates{i,j};
            coords.push_back(board_coordinates);
        }

        std::clog << "create Ship" << std::endl;
        Ship ship{top_left, coords};

        std::clog << "set Ship type" << std::endl;
        ship.setType(obj_ship.at("type")); // a mon avis ca ca va pas marcher, on a pas de conversion int -> CellType

        std::clog << "handlePlaceShip" << std::endl;
        bool result = _game->handlePlaceShip(role_to_turn(player),ship);
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
        // Return a default value
        return Turn::PLAYERONE;
    }
}


nlohmann::json GameState::getGameState(PlayerRole player) const {
    // Return a placeholder or current game state
    // For simplicity, we return currentState directly
    return _game->getState(player);
}