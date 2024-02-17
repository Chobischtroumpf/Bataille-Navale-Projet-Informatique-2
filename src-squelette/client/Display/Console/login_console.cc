#include "login_console.hh"
#include <iostream>

/*
Dans la classe LoginController à quoi correspond checkValidity
Faut il donc une autre méthode pour Ajouter le compte nouvellememnt créé à la BD
Attention il n y a pas encore de fichier.cpp pour LoginController
*/

void LoginConsole::afficherMenuPrincipal() {
    LoginController loginController; 
    int choix;
    std::cout << "1. Se connecter\n";
    std::cout << "2. S'enregistrer\n";
    std::cout << "Entrez votre choix: ";
    std::cin >> choix;
	while (!validCin(choix))
		std::cin >> choix;

// Pas utiliser switch faire tout en boucle while(correcte) if 1 2
    switch(choix) {
        case 1:
            if (seConnecter(loginController)) {
                std::cout << "Connexion réussie!\n";
				// Fonction pour déplacer vers la prochaine fenêtre, MenuPrincipal
            } else {
                std::cout << "Échec de la connexion. Veuillez réessayer.\n";
				LoginConsole::afficherMenuPrincipal();
				return;
            }
            break;
        case 2:
            if (sEnregistrer(loginController)) {
                std::cout << "Enregistrement réussi. Veuillez vous connecter.\n";
				seConnecter();
				return;
            } else {
                std::cout << "Échec de l'enregistrement. Veuillez réessayer.\n";
				LoginConsole::afficherMenuPrincipal();
				return;
            }
            break;
        default:
            std::cout << "Choix invalide. Veuillez réessayer.\n";
            afficherMenuPrincipal();
            break;
    }
}

bool LoginConsole::seConnecter(LoginController& loginController) {
    std::string nomUtilisateur = demanderNomUtilisateur();
    std::string motDePasse = demanderMotDePasse();
    return loginController.attemptLogin(nomUtilisateur, motDePasse); //LC IMPLEMENTER attemptLogin
}

bool LoginConsole::sEnregistrer(LoginController& loginController) {
    std::string nomUtilisateur = demanderNomUtilisateur();
    std::string motDePasse = demanderMotDePasse();
    return loginController.checkValidity(nomUtilisateur, motDePasse); // LC IMPLEMENTER checkValidity
}

std::string LoginConsole::demanderNomUtilisateur() {
    std::string nomUtilisateur;
    std::cout << "Nom d'utilisateur: ";
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, nomUtilisateur);
    return nomUtilisateur;
}

std::string LoginConsole::demanderMotDePasse() { // Ajouter des conditions sur le mdp?
    std::string motDePasse;
    std::cout << "Mot de passe: ";
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, motDePasse);
    return motDePasse;
}

bool LoginConsole::validCin(int choix){
	if(std::cin.fail()) {
        std::cin.clear(); // Efface l'état d'erreur de cin
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vide le buffer d'entrée
        std::cout << "Entrée invalide. Veuillez entrer un nombre.\n";
		return false;       
    }

    else if (choix != 1 && choix != 2) {
        std::cout << "Choix invalide. Veuillez réessayer.\n";
		return false;
    }
	else
		return true;
}
