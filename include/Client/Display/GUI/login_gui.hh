#pragma once

#include <memory>
#include "login_view.hh"
#include "gui.hh"
#include "login_controller.hh"
#include "game_client.hh"

class LoginGUI : public GUI {
public:
    explicit LoginGUI(std::shared_ptr<GameClient> client);
    ReturnInput handleInput() override;
    virtual void display() override {}
    virtual void displayError() override {}
    virtual void update() override {}
    bool seConnecter(LoginController& loginController);
    bool sEnregistrer(LoginController& loginController);
private:
    std::shared_ptr<LoginView> _view;
    std::shared_ptr<LoginController> _login_controller;
    std::shared_ptr<GameClient> _client;
    std::string demanderNomUtilisateur();
    std::string demanderMotDePasse();
    std::string demanderMotDePasseEnregistrement();
};