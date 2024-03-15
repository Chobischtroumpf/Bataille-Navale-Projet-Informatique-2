#include "login_console.hh"

/*
Dans la classe LoginController à quoi correspond checkValidity
Faut il donc une autre méthode pour Ajouter le compte nouvellememnt créé à la BD
Attention il n y a pas encore de fichier.cpp pour LoginController
*/
LoginConsole::LoginConsole(std::shared_ptr<GameClient> gameClient)
    : loginController(gameClient) {
}

ReturnInput LoginConsole::handleInput() {
  int choix;
  bool continuer = true;
  do {
    // system("clear");
    std::cout << "1. Se connecter\n";
    std::cout << "2. S'enregistrer\n";
    std::cout << "Entrez votre choix: ";
    std::cin >> choix;
  } while (!validCin(choix));

  while (continuer) {
    if (choix == 1) {
      if (seConnecter(loginController)) {
        std::cout << "Connexion réussie!\n";
        continuer = false;
        return {ReturnInput::MAIN_MENU, ""};
      } else {
        std::cout << "Échec de la connexion. Veuillez réessayer.\n";
      }
    } else if (choix == 2) {
      if (sEnregistrer(loginController)) {
        std::cout << "Enregistrement réussi.";
        continuer = false;
        return {ReturnInput::MAIN_MENU, ""};
      } else {
        std::cout << "Échec de l'enregistrement. Veuillez réessayer.\n";
      }
    } else {
      std::cout << "Choix invalide. Veuillez réessayer.\n";
    }
  }
  return {ReturnInput::Screen::LOGIN, ""};
}

bool LoginConsole::seConnecter(LoginController &loginController) {
  //system("clear");
  std::cout << "Se connecter\n";
  std::string username = demanderNomUtilisateur();
  std::string password = demanderMotDePasse();
  auto registerFuture = loginController.attemptLogin(username, password);
  return (registerFuture.get());
}

bool LoginConsole::sEnregistrer(LoginController &loginController) {
  //system("clear");
  std::cout << "Enregistrement\n";
  std::string username = demanderNomUtilisateur();
  std::string password = demanderMotDePasseEnregistrement();
  auto registerFuture = loginController.attemptRegister(username, password);
  return (registerFuture.get());
}

std::string LoginConsole::demanderNomUtilisateur() {
  std::string nomUtilisateur;
  std::cout << "Nom d'utilisateur: ";
  if (std::cin.peek() == '\n')
    std::cin.ignore();
  std::getline(std::cin, nomUtilisateur);
  return nomUtilisateur;
}

std::string LoginConsole::demanderMotDePasse() { // Ajouter des conditions sur le mdp?
  std::string motDePasse;
  std::cout << "Mot de passe: ";
  if (std::cin.peek() == '\n')
    std::cin.ignore();
  std::getline(std::cin, motDePasse);
  return motDePasse;
}

std::string LoginConsole::demanderMotDePasseEnregistrement() {
  std::string motDePasse;
  bool isValid = false;

  while (!isValid) {
    std::cout
        << "Mot de passe (minimum 6 caractères, incluant une majuscule): ";
    if (std::cin.peek() == '\n')
      std::cin.ignore();
    std::getline(std::cin, motDePasse);
    // system("clear");

    if (motDePasse.length() >= 6 && contientMajuscule(motDePasse)) {
      isValid = true;
    } else {
      std::cout << "Mot de passe invalide. Assurez-vous qu'il contient au "
                   "moins 6 caractères et une majuscule.\n";
    }
  }
  return motDePasse;
}

bool LoginConsole::validCin(int choix) {
  if (std::cin.fail()) {
    std::cin.clear(); // Efface l'état d'erreur de cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // Vide le buffer d'entrée
    std::cout << "Entrée invalide. Veuillez entrer un nombre.\n";
    return false;
  }

  else if (choix != 1 && choix != 2) {
    std::cout << "Choix invalide. Veuillez réessayer.\n";
    return false;
  } else
    return true;
}

bool LoginConsole::contientMajuscule(const std::string &str) {
  for (char c : str) {
    if (std::isupper(c)) {
      return true;
    }
  }
  return false;
}