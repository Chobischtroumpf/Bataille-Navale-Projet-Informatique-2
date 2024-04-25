#include "gui_review.hh"
#include <qt6/QtWidgets/qpushbutton.h>

Review::Review(std::shared_ptr<ReviewController> review_controller): _review_controller(review_controller){
    setWindowTitle("Review Menu");
    resize(1200, 800);
    setButtons();
    setListView();
    setLayout();
}   

void Review::setButtons(){
    r_game_button = new QPushButton("Lancer le replay");
    connect(r_game_button, &QPushButton::clicked, this, &Review::onRGameButtonClicked);
    back_main_button = new QPushButton("Retour");
    connect(back_main_button, &QPushButton::clicked, this, &Review::onBackMainButtonClicked);
}

void Review::setListView(){
    QStringList game_titles;
    for(auto game: _review_controller->getSessionIdList()){
        QString line = QString("Player1: %1, Player2: %2, Session: %3")
                      .arg(QString::fromStdString(game.id_player1))
                      .arg(QString::fromStdString(game.id_player2))
                      .arg(QString::fromStdString(game.session_id));
        game_titles << line;
    }
    list_model = new QStringListModel(game_titles);    
    //list_model->setStringList(QStringList() << "Player 1" << "Player 2" << "Game type" << "Session ID");    
    game_list_view = new QListWidget(this);
    game_list_view->setModel(list_model);
}

void Review::setLayout(){
    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Selectioner une partie a revoir:"));
    layout->addWidget(game_list_view);
    button_layout = new QHBoxLayout();
    button_layout->addWidget(back_main_button);
    button_layout->addWidget(r_game_button);
    layout->addLayout(button_layout);
}

void Review::onRGameButtonClicked(){
    QModelIndex current_index = game_list_view->currentIndex();
    if (current_index.isValid()) {
        QString selected_text = current_index.data().toString();
        QStringList parts = selected_text.split(",");
        QString session_part = parts.last().trimmed();
        QString session_id = session_part.split(":").last().trimmed();
        std::string selected_game = session_id.toStdString();
        emit goToGameReview(selected_game);
    }
}

void Review::onBackMainButtonClicked(){
    emit goToMainMenu();
}

Review::~Review(){}