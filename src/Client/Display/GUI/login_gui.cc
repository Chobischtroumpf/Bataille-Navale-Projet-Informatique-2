#include "login_gui.hh"

LoginGUI::LoginGUI(std::shared_ptr<GameClient> client) : _client(client) {}

ReturnInput LoginGUI::handleInput() {
    return {ReturnInput::Screen::LOGIN, ""};
}

bool LoginGUI::seConnecter(LoginController &loginController) {

}

bool LoginGUI::sEnregistrer(LoginController &loginController) {

}

std::string LoginGUI::demanderMotDePasse() {

}

std::string LoginGUI::demanderMotDePasseEnregistrement() {

}

std::string LoginGUI::demanderNomUtilisateur() {

}
