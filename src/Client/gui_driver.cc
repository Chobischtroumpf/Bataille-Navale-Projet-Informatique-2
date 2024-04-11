#include "gui_driver.hh"

DriverGui::DriverGui()
    : _game_client{std::make_shared<GameClient>("http://localhost:8080")} {}

void DriverGui::run() {
    _loginWindow = std::make_unique<LoginWindow>(_game_client);
    _loginWindow->show();
}
