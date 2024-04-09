#include "login_console.hh"

LoginConsole::LoginConsole(std::shared_ptr<GameClient> gameClient)
    : _login_controller(gameClient) {}

ReturnInput LoginConsole::handleInput() {
  int choice = makeChoice();
  bool continuer = true;
  

  while (continuer) {
    if (choice == 1) {
      if (seConnecter(_login_controller)) {
        std::cout << "Connexion réussie!\n";
        continuer = false;
        return {ReturnInput::MAIN_MENU, ""};
      } else {
        system("clear");
        choice = makeChoice();
      }
    } else if (choice == 2) {
      if (sEnregistrer(_login_controller)) {
        std::cout << "Enregistrement réussi.\n";
        continuer = false;
        _login_controller.addNotification("", "Welcome new user!");
        return {ReturnInput::MAIN_MENU, ""};
      } else {
          system("clear");
          choice = makeChoice();
      }
    } else {
      std::cout << "Choix invalide. Veuillez réessayer.\n";
    }
  }
  return {ReturnInput::Screen::LOGIN, ""};
}

int LoginConsole::makeChoice(){
  int choice;
  while (!validCin(choice)) {
    std::cout << "1. Se connecter\n";
    std::cout << "2. S'enregistrer\n";
    std::cout << "Entrez votre choice: ";
    std::cin >> choice;
  }
  return choice;
}

bool LoginConsole::seConnecter(LoginController &_login_controller) {
  // system("clear");
  std::cout << "Se connecter\n";
  std::cout << "Type ./exit to back in the menu\n";
  std::string username = demanderNomUtilisateur();
  if (username == "./exit") return false;
  std::string password = demanderMotDePasse();
  auto registerFuture = _login_controller.attemptLogin(username, password);
  return (registerFuture.get());
}

bool LoginConsole::sEnregistrer(LoginController &_login_controller) {
  // system("clear");
  std::cout << "Enregistrement\n";
  std::cout << "Type ./exit to back in the menu\n";
  std::string username = demanderNomUtilisateur();
  if (username == "./exit") return false;
  std::string password = demanderMotDePasseEnregistrement();
  auto registerFuture = _login_controller.attemptRegister(username, password);
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

std::string
LoginConsole::demanderMotDePasse() {
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

bool LoginConsole::validCin(int choice) {
  if (std::cin.fail()) {
    std::cin.clear(); // Efface l'état d'erreur de cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // Vide le buffer d'entrée
    std::cout << "Entrée invalide. Veuillez entrer un nombre.\n";
    return false;
  }

  else if (choice != 1 && choice != 2) {
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
