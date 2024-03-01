#include "menu.hh"
#include <iostream>

/*
Dans la classe LoginController à quoi correspond checkValidity
Faut il donc une autre méthode pour Ajouter le compte nouvellememnt créé à la BD
Attention il n y a pas encore de fichier.cpp pour LC
*/

void Menu::afficherMenuPrincipal() {// Rename Menu en Login???
    //LoginController loginController; // Vérification des logs
    int choix;
    std::cout << "1. Se connecter\n";
    std::cout << "2. S'enregistrer\n";
    std::cout << "Entrez votre choix: ";
    std::cin >> choix;
	while(!validCin(choix)){
		std::cin >> choix;
	}

    switch(choix) {
        case 1:
            if (seConnecter()) {
                std::cout << "Connexion réussie!\n";
				// Fonction pour déplacer vers la prochaine fenêtre(menu)
            } else {
                std::cout << "Échec de la connexion. Veuillez réessayer.\n";
            }
            break;
        case 2:
            if (sEnregistrer()) {
                std::cout << "Enregistrement réussi. Veuillez vous connecter.\n";
				seConnecter(); // CHECK
				return;
            } else {
                std::cout << "Échec de l'enregistrement. Veuillez réessayer.\n";
				afficherMenuPrincipal();
            }
            break;
        default:
            std::cout << "Choix invalide. Veuillez réessayer.\n";
            afficherMenuPrincipal();
            break;
    }
}

bool Menu::seConnecter() {
    std::string nomUtilisateur = demanderNomUtilisateur();
    std::string motDePasse = demanderMotDePasse();

    return true;//loginController.attemptLogin(nomUtilisateur, motDePasse); //LC IMPLEMENTER attemptLogin
}

bool Menu::sEnregistrer() {
    std::string nomUtilisateur = demanderNomUtilisateur();
    std::string motDePasse = demanderMotDePasse();
    return true;//loginController.checkValidity(nomUtilisateur, motDePasse); // LC IMPLEMENTER checkValidity
}

std::string Menu::demanderNomUtilisateur() {
    std::string nomUtilisateur;
    std::cout << "Nom d'utilisateur: ";
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, nomUtilisateur);
    return nomUtilisateur;
}

std::string Menu::demanderMotDePasse() {
    std::string motDePasse;
    std::cout << "Mot de passe: ";
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, motDePasse);
    return motDePasse;
}

bool Menu::validCin(int choix){
	if(std::cin.fail()) { // si jamais cin n'était pas un int
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
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