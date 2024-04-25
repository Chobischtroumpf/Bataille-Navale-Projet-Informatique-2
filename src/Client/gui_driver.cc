#include "gui_driver.hh"
#include "review_controller.hh"
#include "local_board_review.hh"
#include "gui_review.hh"
#include "gui_review_game.hh"

#include <qt6/QtCore/qobject.h>

DriverGui::DriverGui()
    : _game_client{std::make_shared<GameClient>("http://localhost:8080")} {}

void DriverGui::run() {
    showLoginWindow();
}
void DriverGui::showLoginWindow() {
    _loginWindow = std::make_unique<LoginWindow>(_game_client);
    QObject::connect(_loginWindow.get(), &LoginWindow::loginSuccessful, this, &DriverGui::showMainMenu);
    QObject::connect(_loginWindow.get(), &LoginWindow::registrationSuccessful, this, &DriverGui::showMainMenu);
    _loginWindow->show();
}

void DriverGui::showMainMenu() {
    _mainMenuWindow = std::make_unique<MainMenu>(_game_client);
    QObject::connect(_mainMenuWindow.get(), &MainMenu::userDisconnection, this, &DriverGui::showLoginWindow);
    QObject::connect(_mainMenuWindow.get(), &MainMenu::startGameSetting, this, &DriverGui::showGameSettingWindow);
    QObject::connect(_mainMenuWindow.get(), &MainMenu::startChat, this, [this](const std::string &destination) { showChatOutWindow(destination); });
    QObject::connect(_mainMenuWindow.get(), &MainMenu::startReview, this, &DriverGui::showReviewMenu);
    _mainMenuWindow->show();
}

void DriverGui::showChatOutWindow(const std::string &destination) {
    if (_chatOutWindow) {
        _chatOutWindow->close();  // Ferme l'instance existante, si elle existe
    }
    _chatOutWindow = std::make_unique<ChatOut>(destination, _game_client); // Crée une nouvelle instance avec la bonne destination
    QObject::connect(_chatOutWindow.get(), &ChatOut::goBackToMenu, this, &DriverGui::showMainMenu);
    _chatOutWindow->show();
}

void DriverGui::showGameWindow(std::string gameId) {
    bool is_commander = _lobbyWindow->isCommanderMode();
    std::string session_id = _lobbyWindow->getSessionId();
    _gameWindow = std::make_unique<Game>(_game_client, session_id, is_commander);
    connect(_gameWindow.get(), &Game::gameFinished, this, &DriverGui::showMainMenu);
    _gameWindow->show();
}

void DriverGui::showGameSettingWindow() {
    _gameSettingWindow = std::make_shared<GameSetting>(_game_client);
    QObject::connect(_gameSettingWindow.get(), &GameSetting::goBackToMenu, this, &DriverGui::showMainMenu);
    QObject::connect(_gameSettingWindow.get(), &GameSetting::goToLobby, this, &DriverGui::showLobbyWindow);
    _gameSettingWindow->show();
}

void DriverGui::showLobbyWindow(std::string gameId, bool admin = false) {
    _lobbyWindow = std::make_unique<Lobby>(gameId, _game_client, admin, _gameSettingWindow);
    QObject::connect(_lobbyWindow.get(), &Lobby::goBackToMenu, this, &DriverGui::showMainMenu);
    QObject::connect(_lobbyWindow.get(), &Lobby::launchGame, this, &DriverGui::showGameWindow);
    _lobbyWindow->show();
}

void DriverGui::showReviewMenu(){
    std::shared_ptr<ReviewController> review_controller = std::make_shared<ReviewController>(_game_client);
    _review_menu = std::make_unique<Review>(review_controller);
    QObject::connect(_review_menu.get(), &Review::goToMainMenu, this, &DriverGui::showMainMenu);
    QObject::connect(_review_menu.get(), &Review::goToGameReview, this, &DriverGui::showReviewGame);
    _review_menu->show();
}

void DriverGui::showReviewGame(std::string gameId){
    std::shared_ptr<LocalBoardReview> board = std::make_shared<LocalBoardReview>(gameId);
    std::shared_ptr<ReviewGameController> reviewg_controller = std::make_shared<ReviewGameController>(_game_client, board);
    _review_game = std::make_unique<ReviewGame>(board, reviewg_controller);
    QObject::connect(_review_game.get(), &ReviewGame::goToMainMenu, this, &DriverGui::showMainMenu);
    _review_game->show();
}