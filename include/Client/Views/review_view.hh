#pragma once

#include "view.hh"
#include "game_client.hh"


class ReviewView: public View
{
private:
    std::shared_ptr<GameClient> _game_client;

public:
    ReviewView(std::shared_ptr<GameClient> game_client);
    ~ReviewView() override{}
    void update() override{}
};

