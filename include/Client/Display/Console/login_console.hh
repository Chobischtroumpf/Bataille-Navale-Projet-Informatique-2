#pragma once

#include <memory>
#include <cstdlib>
#include <limits>
#include <iostream>

#include "login_view.hh"
#include "console.hh"
#include "login_controller.hh"
#include "display.hh"

class LoginConsole : public Console {
public:
  ReturnInput handleInput() override;                       
  virtual void display() override {}
  virtual void displayError() override {}
  virtual void update() override {}
  bool seConnecter(LoginController& loginController);
  bool sEnregistrer(LoginController& loginController);
private:
  std::shared_ptr<LoginView> _view; // Déjà là, comment l'utiliser ?
  LoginController loginController;
  std::string demanderNomUtilisateur();
  std::string demanderMotDePasse();
  std::string demanderMotDePasseEnregistrement();
  bool validCin(int choix);
  bool contientMajuscule(const std::string& str);
};