#pragma once

#include "../../Views/login_view.hh"
#include <memory>
#include "../console.hh"
#include <string>
#include "../../Controllers/login_controller.hh"
#include "../../display.hh"

class LoginConsole : public Console {
public:
  explicit LoginConsole(const std::string& baseUri);
  ReturnInput handle_input() override;                       
  virtual void display() override {}
  virtual void display_error() override {}
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

