#include "review_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"


ReviewController::ReviewController(std::shared_ptr<GameClient> client)
    : _client{client}{
        requestSessionsIds();
    }

void ReviewController::requestSessionsIds(){
    // Watch format, need to init a Session_info instance
    //_session_id_list = _client->GetGames();
}

const std::vector<Session_info>& ReviewController::getSessionIdList(){
    return _session_id_list;
}

const Session_info ReviewController::getSessionInfo(int index){
    return _session_id_list[index-1];
}

