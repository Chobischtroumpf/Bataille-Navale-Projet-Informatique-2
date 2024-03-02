#ifndef MENU_HH
#define MENU_HH

#include <string>
#include <limits>
//#include "src-squelette/client/Controllers/login_controller.hh" // add bon chemin d'accès une fois fichier bon endroit

class Menu {
public:
    void afficherMenuPrincipal();
    bool seConnecter();
    bool sEnregistrer();
private:
    std::string demanderNomUtilisateur();
    std::string demanderMotDePasse();
	bool validCin(int choix);
};

#endif // MENU_H
