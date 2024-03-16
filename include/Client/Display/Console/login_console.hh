#pragma once

#include <memory>
#include <cstdlib>
#include <limits>
#include <iostream>

#include "login_view.hh"
#include "console.hh"
#include "login_controller.hh"
#include "display.hh"

#include "game_client.hh"

class LoginConsole : public Console {
private:
  std::shared_ptr<LoginView> _view;
  LoginController _login_controller;
  std::string demanderNomUtilisateur();
  std::string demanderMotDePasse();
  std::string demanderMotDePasseEnregistrement();
  bool validCin(int choix);
  bool contientMajuscule(const std::string& str);

public:
  explicit LoginConsole(std::shared_ptr<GameClient> gameClient);
  ReturnInput handleInput() override;                       
  virtual void display() override {}
  virtual void displayError() override {}
  virtual void update() override {}
  bool seConnecter(LoginController& loginController);
  bool sEnregistrer(LoginController& loginController);
};
