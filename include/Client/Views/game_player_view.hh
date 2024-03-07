#pragma once

#include "../view.hh"
#include <vector>
#include <string>
#include <tuple>


class GamePlayerView: public View {
public:
    std::vector<std::string> getPlayerName();
    void update() override;
private:
    std::vector<std::string> player_name;
};