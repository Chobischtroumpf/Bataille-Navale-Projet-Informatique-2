#pragma once

#include "board_coordinates.hh"
#include "controller.hh"
#include "display.hh"
#include "local_board_review.hh"
#include "ship.hh"

struct Session_info{
    const std::string session_id;
    const std::string id_player1;
    const std::string id_player2;
    const std::string game_type;
};

class ReviewController : public Controller
{
private:
    std::shared_ptr<GameClient> const _client;
    std::vector<Session_info> _session_id_list;

public:
    ReviewController(std::shared_ptr<GameClient> client);
    ReviewController(const ReviewController &) = default;
    ReviewController(ReviewController &&) = default;
    ReviewController &operator=(const ReviewController &) = default;
    ReviewController &operator=(ReviewController &&) = default;
    virtual ~ReviewController() = default;

    /**
     * Request to the server the sessions ids of the game played by a user.
     */
    void requestSessionsIds(const std::string &player_id);

    /**
     * Get the session id list.
     */
    const std::vector<Session_info>& getSessionIdList();

    /**
     * Get the session info.
     */
    const Session_info getSessionInfo(int index);

    const size_t getSessionsSize();

};

