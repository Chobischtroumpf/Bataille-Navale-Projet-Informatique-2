#include "../../../../include/client/Display/Console/main_menu_console.hh"

MainMenuConsole::MainMenuConsole(std::shared_ptr<MainMenuView> view): _view(view) {}

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
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 1:
            std::cout << "║ (1) Choose game mode 1" << std::endl;
            std::cout << "║ (2) Chosse game mode 2" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 2:
            std::cout << "║ Enter a friend ID to send a game request!" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 3:
            std::cout << "║ Enter a friend ID to send a friend request!" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
    }
}


void MainMenuConsole::display() {
    displayFriends();
    displayNotifications();
    displayOptions(0);
}

void MainMenuConsole::display_error() {

}

void MainMenuConsole::update() {

}

void MainMenuConsole::handle_input() {
    int input;
    std::cin >> input;
    switch (input) {
        int mode; int friend_id;
        case 1:
            displayFriends(); displayNotifications(); displayOptions(1);
            std::cin >> mode; // game
            displayFriends(); displayNotifications(); displayOptions(2);
            std::cin >> friend_id; // friend_id
            //_controller->createGame(mode, friend_id)
            // Need to change the display and view after this point
            display(); // Delete after adding the previous points
            break;
        case 2:
            displayFriends(); displayNotifications(); displayOptions(3);
            std::cin >> friend_id;
            display();
            break;
        case 3:
            display();
            break;
        case 4:
            _friendlist_position += 4;
            display();
    }
}