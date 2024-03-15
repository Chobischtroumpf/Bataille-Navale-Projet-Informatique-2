#include "main_menu_console.hh"
#include <iostream>


MainMenuConsole::MainMenuConsole(std::shared_ptr<GameClient> client) {
    _view = std::make_shared<MainMenuView>(client);
    _controller = std::make_shared<MainMenuController>(client);
}

void MainMenuConsole::displayFriends() {
    std::cout << "╔════════════╗\n║Friends List║\n╠════"
                 "════════╩════════════════════════════════════════════════════════════════════╪\n║\n║ ";
    int friend_counter = 1;
    for (auto i: _view->getFriends()) {
        // On récupère le status du joueur
        std::string status = "";
        std::string color = "";
        if (friend_counter <= _friendlist_position && friend_counter >= _friendlist_position-3) {
            switch (get<1>(i)){
                case 0:
                    status = "▶ playing"; color = "\033[0;32m";
                    break;
                case 1:
                    status = "◌ idle"; color = "\033[0;33m";
                    break;
                case 2:
                    status = "● in lobby"; color = "\033[0;34m";
                    break;
                case 3:
                    status = "○ offline";
            }
            // On affiche le pseudo & son statut
            std::cout << get<0>(i) << " " << color << status << "\033[0m । ";
        }
        friend_counter++;
    }
    // On reset le compteur du menu défilant
    if (friend_counter <= _friendlist_position) {
        _friendlist_position = 0;
    }
    std::cout << "\n║\n";
}

void MainMenuConsole::resetFriendListPosition() {
    _friendlist_position = 4;
}

void MainMenuConsole::displayNotifications() {
    std::cout << "╠═════════════╗\n║Notifications║\n╠══"
                 "═══════════╩═══════════════════════════════════════════════════════════════════╪\n";
    for (auto i: _view->getNotifications()) {
        std::cout << "║ \033[0;33m◈\033[0m " << i <<
        "\n╠═════════════════════════════════════════════════════════════════════════════════╪\n";
    }

}

void MainMenuConsole::displayOptions(int mode) {
    std::cout << "║\n║\n╠════════════════════════╗\n║What do you want to do ?║\n╠══"
                 "══════════════════════╩════════════════════════════════════════════════════════╪\n";
    switch (mode) {
        case 0:
            std::cout << "║ (1) Create a new game ⌨" << std::endl;
            std::cout << "║ (2) Add a friend ⌘" << std::endl;
            std::cout << "║ (3) Refresh ⌛" << std::endl;
            std::cout << "║ (4) Show more friends ☳" << std::endl;
            std::cout << "║ (5) Chat with a friend ✍" << std::endl;
            std::cout << "║ (6) Log out ↆ" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 1:
            std::cout << "║ (1) Choose game mode 1" << std::endl;
            std::cout << "║ (2) Choose game mode 2" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 2:
            std::cout << "║ Enter a friend ID to send a game request!" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 3:
            std::cout << "║ Enter a username to send a request!" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 4:
            std::cout << "║ Invalid option! Choose from the list below" << std::endl;
            std::cout << "║ (1) Create a new game ⌨" << std::endl;
            std::cout << "║ (2) Add a friend ⌘" << std::endl;
            std::cout << "║ (3) Refresh ⌛" << std::endl;
            std::cout << "║ (4) Show more friends ☳" << std::endl;
            std::cout << "║ (5) Chat with a friend" << std::endl;
            std::cout << "║ (6) Log out ↆ" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
    }
}


void MainMenuConsole::display() {
    // Gets all the updated data from the view and displays them
    std::system("clear");
    displayFriends();
    displayNotifications();
    displayOptions(_current_option);
}

void MainMenuConsole::displayError() {

}

void MainMenuConsole::update() {

}

ReturnInput MainMenuConsole::handleInput() {
    int input;
    std::cin >> input;
    std::string username;

    if (std::cin.fail()) {
        std::system("clear");
        resetFriendListPosition();
        _current_option = 4;
        std::cin.clear(); // To clear the error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores the invalid input
        return {ReturnInput::MAIN_MENU, ""};
    }


    switch (input) {
        case 1: { // Create a game
            return {ReturnInput::GAME_CREATION, ""};
        }
        case 2: {// Add a friend
            _current_option = 3;
            if (_friendlist_position == 0) {resetFriendListPosition();}
            display();
            _current_option = 0;
            std::cin >> username;
            _controller->addFriend(username);
            resetFriendListPosition();
            break;
        }
        case 3: {// Refresh
            _current_option = 0;
            resetFriendListPosition();
            break;
        }
        case 4: {// Display more friends
            _current_option = 0;
            _friendlist_position += 4;
            break;
        }
        case 5: {// Chat with a friend
            _current_option = 3;
            if (_friendlist_position == 0) {resetFriendListPosition();}
            display();
            _current_option = 0;
            std::cin >> username;
            return {ReturnInput::CHAT,username};
        }
        case 6:
            std::system("clear");
            return {ReturnInput::LOGIN, ""};
    }
    // In case no game was created, displays the same menu after an input
    return {ReturnInput::MAIN_MENU, ""};
}