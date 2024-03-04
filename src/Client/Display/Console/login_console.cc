#include "login_console.hh"

/*
Dans la classe LoginController à quoi correspond checkValidity
Faut il donc une autre méthode pour Ajouter le compte nouvellememnt créé à la BD
Attention il n y a pas encore de fichier.cpp pour LoginController
*/

ReturnInput LoginConsole::handleInput() {
  LoginController loginController;
  int choix;
  bool continuer = true;

  while (!validCin(choix)) {
    system("clear");
    std::cout << "1. Se connecter\n";
    std::cout << "2. S'enregistrer\n";
    std::cout << "Entrez votre choix: ";
    std::cin >> choix;
  }

  while (continuer) {
    if (choix == 1) {
      if (seConnecter(loginController)) {
        std::cout << "Connexion réussie!\n";
        // Fonction pour déplacer vers la prochaine fenêtre, MenuPrincipal
        continuer = false;
        return {ReturnInput::MAIN_MENU, ""};
      } else {
        std::cout << "Échec de la connexion. Veuillez réessayer.\n";
      }
    } else if (choix == 2) {
      if (sEnregistrer(loginController)) {
        std::cout << "Enregistrement réussi. Veuillez vous connecter.\n";
        choix = 1;
      } else {
        std::cout << "Échec de l'enregistrement. Veuillez réessayer.\n";
      }
    } else {
      std::cout << "Choix invalide. Veuillez réessayer.\n";
    }
  }
  return {ReturnInput::Screen::MAIN_MENU, ""};
}

bool LoginConsole::seConnecter(LoginController &loginController) {
  system("clear");
  std::cout << "Se connecter\n";
  std::string nomUtilisateur = demanderNomUtilisateur();
  std::string motDePasse = demanderMotDePasse();
  return loginController.attemptLogin(
      nomUtilisateur, motDePasse); // LC IMPLEMENTER attemptLogin
}

bool LoginConsole::sEnregistrer(LoginController &loginController) {
  system("clear");
  std::cout << "Enregistrement\n";
  std::string nomUtilisateur = demanderNomUtilisateur();
  std::string motDePasse = demanderMotDePasseEnregistrement();
  return loginController.checkValidity(
      nomUtilisateur, motDePasse); // LC IMPLEMENTER checkValidity
}

std::string LoginConsole::demanderNomUtilisateur() {
  std::string nomUtilisateur;
  std::cout << "Nom d'utilisateur: ";
  if (std::cin.peek() == '\n')
    std::cin.ignore();
  std::getline(std::cin, nomUtilisateur);
  return nomUtilisateur;
}

std::string
LoginConsole::demanderMotDePasse() { // Ajouter des conditions sur le mdp?
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