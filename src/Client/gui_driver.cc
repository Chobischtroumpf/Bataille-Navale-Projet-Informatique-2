#include "gui_driver.hh"
#include <qt6/QtCore/qobject.h>

DriverGui::DriverGui(std::string server_address)
    : _game_client{std::make_shared<GameClient>(server_address)} {}

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
    QObject::connect(_mainMenuWindow.get(), &MainMenu::startLobby, this, [this] (std::string gameId) { showLobbyWindow(gameId, false); });
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
    int selected_faction = _lobbyWindow->getSelectedFaction();
    bool is_spectator = _lobbyWindow->isSpectatorMode();
    _gameWindow = std::make_unique<Game>(_game_client, session_id, selected_faction, is_commander, is_spectator);
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