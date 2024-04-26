#include "gui_review_game.hh"

/*********************
 * BOARD REVIEW FRAME*
**********************/

BoardReviewFrame::BoardReviewFrame(ReviewGame *parent, std::shared_ptr<LocalBoardReview> board, bool my_side):
   _parent(parent), _board(board), _my_side(my_side){
    setMouseTracking(true);
}

QBrush BoardReviewFrame::getTileColor(CellType cell) {
    QBrush brush;
    switch (cell) {
        case CellType::UNDAMAGED_SHIP:
          brush = Qt::gray;
          break;
        case CellType::HIT_SHIP:
          brush = Qt::red;
          break;
        case CellType::OCEAN:
          brush = Qt::cyan;
          break;
        case CellType::SCANNED_SHIP:
          brush = Qt::yellow;
          break;
        case CellType::SUNK_SHIP:
          brush = Qt::darkRed;
          break;
        case CellType::HIT_MINE:
          brush = Qt::black;
          break;
        case CellType::SCANNED_MINE:
          brush = Qt::darkYellow;
          break;
        case CellType::WATER:
          brush = Qt::blue;
          break;
        case CellType::UNDAMAGED_MINE:
          brush = Qt::black;
          break;
        default:
          brush = Qt::white;
          break;
        }
    return brush;
}   


void BoardReviewFrame::drawCell(QPainter &painter, int x, int y, CellType cell) {
    QBrush brush = getTileColor(cell);
    painter.fillRect(x * 50, y * 50, 50, 50, brush);
    painter.setPen(Qt::white);
    painter.drawRect(x * 50, y * 50, 50, 50);
}


void BoardReviewFrame::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event); // To avoid unused parameter warning
  QPainter painter(this);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        drawCell(painter, i, j, _board->cellType(_my_side, BoardCoordinates(i, j)));
    }
  }
}


void BoardReviewFrame::mousePressEvent(QMouseEvent *event) {}

void BoardReviewFrame::mouseMoveEvent(QMouseEvent *event) {}

void BoardReviewFrame::keyPressEvent(QKeyEvent *event) {}

/***************
* BOARD REVIEW *
****************/


ReviewGame::ReviewGame(std::shared_ptr<LocalBoardReview> board, std::shared_ptr<ReviewGameController> controller, bool my_side):
    _board(board), _controller(controller), _commander_mode(my_side){
    setWindowTitle("Game Review");
    resize(1200, 800);
    _my_frame = new BoardReviewFrame(this, _board, true);
    _their_frame = new BoardReviewFrame(this, _board, false);
    _my_frame->setFixedSize(500, 500);
    _their_frame->setFixedSize(500, 500);
    setButtons();
    setLayout();
}

void ReviewGame::setButtons(){
    next_button = new QPushButton("Suivant");
    connect(next_button, &QPushButton::clicked, this, &ReviewGame::onNextButtonClicked);
    previous_button = new QPushButton("Précédant");
    connect(previous_button, &QPushButton::clicked, this, &ReviewGame::onPreviousButtonClicked);
    back_main_button = new QPushButton("Quiter");
    connect(back_main_button, &QPushButton::clicked, this, &ReviewGame::onBackMainButtonClicked);
}

void ReviewGame::setLayout(){
    layout = new QVBoxLayout(this);
    label_layout = new QHBoxLayout();
    boards_layout = new QHBoxLayout();
    button_layout = new QHBoxLayout();
    std::string game_label = "Revoir la partie: " + _board->getSessionId();
    layout->addWidget(new QLabel(QString::fromStdString(game_label)));
    label_layout->addWidget(new QLabel("Joueur 1:"));
    label_layout->addWidget(new QLabel("Joueur 2"));
    layout->addLayout(label_layout);
    boards_layout->addWidget(_my_frame);
    boards_layout->addWidget(_their_frame);
    layout->addLayout(boards_layout);
    button_layout->addWidget(previous_button);
    button_layout->addWidget(next_button);
    layout->addLayout(button_layout);
    layout->addWidget(back_main_button);
}


void ReviewGame::onNextButtonClicked(){
  std::clog << "clicked next" << std::endl;
    _controller->setNextMove();
}

void ReviewGame::onPreviousButtonClicked(){
  std::clog << "clicked previous" << std::endl;
    _controller->setPreviousMove();
}

void ReviewGame::onBackMainButtonClicked(){
    emit goToMainMenu();
}

ReviewGame::~ReviewGame(){
    delete _my_frame;
    delete _their_frame;
    delete next_button;
    delete previous_button;
    delete back_main_button;
    delete label_layout;
    delete boards_layout;
    delete button_layout;
    delete layout;

}

BoardReviewFrame::~BoardReviewFrame(){

}
