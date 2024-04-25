#include "review_console.hh"
#include "local_board_commander.hh"
#include "not_implemented_error.hh"

using std::string;


ReviewConsole::ReviewConsole(std::ostream &out, std::istream &in, std::shared_ptr<ReviewController> controller)
    : _out{out}, _in{in}, _controller{std::move(controller)}{}

void ReviewConsole::display(){
    std::system("clear");
    displayGameList();
}

void ReviewConsole::displayGameList(){
    const std::vector<Session_info> session_id_list = _controller->getSessionIdList();
    std::cout << "┌────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Select a game to review: " << "\n";
    std::cout << "├────────────────────────────────────────────────────────────────────────────────┤\n";
    int i=0;
    for (const auto &session : session_id_list) {
        i += 1;
        if(i==6){break;}
        std::cout << "│ " << i << ") " "\033[1m" << session.game_type << ", " 
                  << "\033[1m" << session.id_player1 << ", "
                  << "\033[1m" << session.id_player2 << "\n";
        std::cout << "│ \033[0m" << session.session_id << "\n";
    }
    std::cout << "└────────────────────────────────────────────────────────────────────────────────┘\n";
    std::cout << "Type the number of the game you want to select or './exit' to quit: \n";
}

ReturnInput ReviewConsole::handleInput(){
    std::string input;
    int number;
    while (true) {
        std::getline(std::cin, input);
        if (input == "./exit") {
            std::cout << "Exiting review...\n";
            return {ReturnInput::MAIN_MENU, ""};
        } 
        std::istringstream iss(input);
        iss >> number;
        if (iss.eof() && number >= 1 && number <= 5) {
                const Session_info session = _controller->getSessionInfo(number);
                return {ReturnInput::REVIEW_GAME, session.session_id};
        } 
        else if(input != ""){
            std::cout << "Wrong input, must be an number in the list, try angain:" << std::endl;
            return {ReturnInput::REVIEW_GAME, "session.session_id"};
        }
    }
}
