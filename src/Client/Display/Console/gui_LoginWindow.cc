#include "gui_LoginWindow.hh"
#include <qt6/QtWidgets/qpushbutton.h>

LoginWindow::LoginWindow(std::shared_ptr<GameClient> gameClient) : login_controller(gameClient) {
    // QLineEdit() pour écrire
    usernameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Cache le mot de passe

    // QPushButton pour appuyer sur un bouton
    loginButton = new QPushButton("Se Connecter");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    registerButton = new QPushButton("S'enregistrer");
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
    // Ajout le layout horizontal avec les deux choix au layout du dessus
    layout->addLayout(buttonLayout);
}


void LoginWindow::onLoginButtonClicked() {
    QString username = usernameLineEdit->text(); //on recup la ligne de texte que l'utilisateur a tapé
    QString password = passwordLineEdit->text(); 
    std::string str_username = username.toStdString();
    std::string str_password = password.toStdString();

    auto registerFuture = login_controller.attemptLogin(str_username, str_password);
    if (registerFuture.get())
        this->close();
    else
        std::cout<< "échec de la connexion" << std::endl;
}

void LoginWindow::onRegisterButtonClicked() {
    QString username = usernameLineEdit->text(); // stock dans username ce qui a été écrit dans usernameLine
    QString password = passwordLineEdit->text(); // same ici pr le mdp
    std::string str_username = username.toStdString();
    std::string str_password = password.toStdString();

    auto registerFuture = login_controller.attemptRegister(str_username, str_password);
    if (registerFuture.get())
        this->close();
    else
        std::cout<< "échec de la connexion" << std::endl;
}