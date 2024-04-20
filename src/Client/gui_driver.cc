#include "gui_driver.hh"
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

void DriverGui::showGameWindow() {
    _gameWindow = std::make_unique<Game>(_game_client);
    _gameWindow->show();
}

void DriverGui::showGameSettingWindow() {
    _gameSettingWindow = std::make_unique<GameSetting>(_game_client);
    QObject::connect(_gameSettingWindow.get(), &GameSetting::goBackToMenu, this, &DriverGui::showMainMenu);
    //QObject::connect(_gameSettingWindow.get(), &GameSetting::goToLobby, this, &DriverGui::showLobbyWindow);
    _gameSettingWindow->show();
}