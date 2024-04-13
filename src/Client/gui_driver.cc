#include "gui_driver.hh"
#include <qt6/QtCore/qobject.h>

DriverGui::DriverGui()
    : _game_client{std::make_shared<GameClient>("http://localhost:8080")} {}

void DriverGui::run() {
    showLoginWindow();
}
void DriverGui::showLoginWindow(){
      _loginWindow = std::make_unique<LoginWindow>(_game_client);
      QObject::connect(_loginWindow.get(), &LoginWindow::loginSuccessful, this, &DriverGui::showMainMenu);
      QObject::connect(_loginWindow.get(), &LoginWindow::registrationSuccessful, this, &DriverGui::showMainMenu);
      _loginWindow->show();
}

void DriverGui::showMainMenu() {
    _mainMenuWindow = std::make_unique<MainMenu>(_game_client);
    QObject::connect(_mainMenuWindow.get(), &MainMenu::userDisconnection, this, &DriverGui::showLoginWindow);
    QObject::connect(_mainMenuWindow.get(), &MainMenu::startChat, this, &DriverGui::showChatOutWindow);
    _mainMenuWindow->show();
}

void DriverGui::showChatOutWindow() {
    _chatOutWindow = std::make_unique<ChatOut>(_destination, _game_client);
    QObject::connect(_chatOutWindow.get(), &ChatOut::goBackToMenu, this, &DriverGui::showMainMenu);
    _chatOutWindow->show();
}
