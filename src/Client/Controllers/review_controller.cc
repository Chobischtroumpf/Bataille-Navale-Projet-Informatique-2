#include "review_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"


ReviewController::ReviewController(std::shared_ptr<GameClient> client)
    : _client{client}{
        requestSessionsIds();
}

void ReviewController::requestSessionsIds(){
    std::future<std::string> future_json = _client->GetGames();
    nlohmann::json json_data = future_json.get();
    nlohmann::json json_data;
    // Watch format, need to init a Session_info instance
    //_session_id_list = _client->GetGames();
}

const std::vector<Session_info>& ReviewController::getSessionIdList(){
    return _session_id_list;
}

const Session_info ReviewController::getSessionInfo(int index){
    return _session_id_list[index-1];
}

