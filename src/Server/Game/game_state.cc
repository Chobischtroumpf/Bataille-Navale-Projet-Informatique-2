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
    return game->handle_fire(role_to_turn(player),board_coordinates);

}

bool GameState::handle_place_ship(PlayerRole player, const nlohmann::json& move){
    /*if (move.size() != 5){
        //handle the number of placement is incorrect
        return false;
    }*/
    for (const auto& obj : move["ships"]) {
        std::string x = obj["anchor"]["x"].get<std::string>();
        size_t y = obj["anchor"]["y"];
        int length = obj["length"];
        bool vertical = obj["vertical"];

        // Create ShipCoordinates object using the extracted values
        ShipCoordinates ship_coordinates;

        if (vertical){
            ship_coordinates.set(BoardCoordinates::parseX(x).value(),y-1,VERTICAL,static_cast<ShipType>(length));
        } else{
            ship_coordinates.set(BoardCoordinates::parseX(x).value(),y-1,HORIZONTAL,static_cast<ShipType>(length));
        }

        bool result = game->handle_place_ship(role_to_turn(player),ship_coordinates);
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
    return game->get_state(player);
}
