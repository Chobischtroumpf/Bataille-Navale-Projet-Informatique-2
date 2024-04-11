#include "gui_LoginWindow.hh"

LoginWindow::LoginWindow() {
    // QLineEdit() pour écrire
    usernameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Cache le mot de passe

    // QPushButton pour appuyer sur un bouton
    loginButton = new QPushButton("Se Connecter");

    // donne une logique à ce qui se passe quand on click sur le boutton 
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);

    // disposition des différents objets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Nom d'utilisateur :"));
    layout->addWidget(usernameLineEdit);
    layout->addWidget(new QLabel("Mot de passe :"));
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
}

void LoginWindow::onLoginButtonClicked() {
    QString username = usernameLineEdit->text(); // stock dans username ce qui a été écrit dans usernameLine
    QString password = passwordLineEdit->text(); // same ici pr le mdp

    // Logique de connexion ici
    // ...
}