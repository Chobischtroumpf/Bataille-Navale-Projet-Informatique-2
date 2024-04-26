#pragma once

#include "board_coordinates.hh"
#include "controller.hh"
#include "display.hh"
#include "local_board_review.hh"
#include "ship.hh"

class ReviewGameController : public Controller
{
private:
    std::shared_ptr<GameClient> const _client;
    std::shared_ptr<LocalBoardReview> _board;
    std::vector<nlohmann::json> _move_list;
    size_t _current_move;

public:
    ReviewGameController(std::shared_ptr<GameClient> client, std::shared_ptr<LocalBoardReview> board);
    ReviewGameController(const ReviewGameController &) = default;
    ReviewGameController(ReviewGameController &&) = default;
    ReviewGameController &operator=(const ReviewGameController &) = default;
    ReviewGameController &operator=(ReviewGameController &&) = default;
    virtual ~ReviewGameController() = default;

    /**
     * Request to the server the game move list.
     */
    void requestMovesList();

    /**
     * Initialize the local board with the first move.
     */
    void initLocalBoard();

    /**
     * Set the local board with the next move of the game.
     */
    void setNextMove();

    /**
     * Set the local board with the previous move of the game.
     */
    void setPreviousMove();

};