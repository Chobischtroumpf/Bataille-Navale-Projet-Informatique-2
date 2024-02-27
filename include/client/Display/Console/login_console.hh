#pragma once

#include "../../Views/login_view.hh"
#include <memory>
#include "../console.hh"
#include <string>
#include "../../Controllers/login_controller.hh"
#include "../../display.hh"

class LoginConsole : public Console {
public:
  ReturnInput afficherMenuPrincipal();                       
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