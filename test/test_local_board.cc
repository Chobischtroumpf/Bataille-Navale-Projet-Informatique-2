#include "local_board_commander.hh"

#include <iostream>
#include <cassert>

void test_local_board() {
    std::cout << "Testing LocalBoardCommander" << std::endl;
    
    // Create a LocalBoardCommander
    LocalBoardCommander lbc({});
    assert(lbc.myTurn() == true);
    assert(lbc.isFinished() == false);
    assert(lbc.isVictory() == false);
    assert(lbc.width() == 10);
    assert(lbc.height() == 10);
    assert(lbc.mode() == GameMode::CLASSIC);
    
    std::cout << "LocalBoardCommander tests passed" << std::endl;
}