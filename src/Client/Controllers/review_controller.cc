#include "review_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"


ReviewController::ReviewController(std::shared_ptr<GameClient> client)
    : _client{client}{
        requestSessionsIds();
}

void ReviewController::requestSessionsIds(){
    std::future<njson> future_json = _client->GetGames();
    nlohmann::json json_data = future_json.get();
    for (const auto& session : json_data) {
        Session_info session_info;
        session_info.session_id = session["sessionId"].get<std::string>();
        session_info.id_player1 = session["player1Id"].get<std::string>();
        session_info.id_player2 = session["player2Id"].get<std::string>();
        _session_id_list.push_back(session_info);
       }
}

const std::vector<Session_info>& ReviewController::getSessionIdList(){
    return _session_id_list;
}

const Session_info ReviewController::getSessionInfo(int index){
    return _session_id_list[index-1];
}

