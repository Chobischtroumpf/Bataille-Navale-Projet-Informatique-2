#include "gui_LoginWindow.hh"
#include <qt6/QtWidgets/qpushbutton.h>

LoginWindow::LoginWindow(std::shared_ptr<GameClient> gameClient) : login_controller(gameClient) {
    // QLineEdit() pour écrire
    usernameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Cache le mot de passe

    // QPushButton pour appuyer sur un bouton
    loginButton = new QPushButton("Se Connecter");
    loginButton->setAutoDefault(true);
    loginButton->setDefault(true); 
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);

    registerButton = new QPushButton("S'enregistrer");
    registerButton->setAutoDefault(true);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);

    // disposition des différents objets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Nom d'utilisateur :"));
    layout->addWidget(usernameLineEdit);
    layout->addWidget(new QLabel("Mot de passe :"));
    layout->addWidget(passwordLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    layout->addLayout(buttonLayout);
}



void LoginWindow::onLoginButtonClicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text(); 
    std::string str_username = username.toStdString();
    std::string str_password = password.toStdString();

    auto loginFuture = login_controller.attemptLogin(str_username, str_password);
    if (loginFuture.get()){
        emit loginSuccessful();
        this->close();
        login_controller.addNotification(str_username, "Connexion réussie.");
    }
    else {
        std::cout << "Échec de la connexion" << std::endl;
        QMessageBox::warning(this, "Connexion", "Échec de la connexion. Veuillez réessayer.");
        login_controller.addNotification(str_username, "Échec de la connexion. Veuillez réessayer.");
    }
}


bool LoginWindow::isPasswordValid(std::string password) {
    if (password.length() >= 6 && contientMajuscule(password)) {
      return true;
    } else {
      return false;
    }
}
void LoginWindow::onRegisterButtonClicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();


    std::string str_username = username.toStdString();
    std::string str_password = password.toStdString();

    if (!isPasswordValid(str_password)) {
        QMessageBox::warning(this, "Enregistrement", "Mot de passe invalide. Minimum 6 caractères dont une majuscule.");
        return;
    }
    auto registerFuture = login_controller.attemptRegister(str_username, str_password);
    if (registerFuture.get()) {
        emit registrationSuccessful();
        this->close();
        login_controller.addNotification(str_username, "Enregistrement réussi.");
    } else {
        QMessageBox::warning(this, "Enregistrement", "Échec de l'enregistrement. Veuillez réessayer.");
        login_controller.addNotification(str_username, "Échec de l'enregistrement. Veuillez réessayer.");
    }
}

bool LoginWindow::contientMajuscule(const std::string &str) {
  for (char c : str) {
    if (std::isupper(c)) {
      return true;
    }
  }
  return false;
}