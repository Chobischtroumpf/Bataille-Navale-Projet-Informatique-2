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
    _my_frame->setFixedSize(500, 500);
    setButtons();
    setLayout();
}

void ReviewGame::setButtons(){
    next_button = new QPushButton("Next");
    connect(next_button, &QPushButton::clicked, this, &ReviewGame::onNextButtonClicked);
    previous_button = new QPushButton("Next");
    connect(previous_button, &QPushButton::clicked, this, &ReviewGame::onPreviousButtonClicked);
    back_main_button = new QPushButton("Retour");
    connect(back_main_button, &QPushButton::clicked, this, &ReviewGame::onBackMainButtonClicked);
}

void ReviewGame::setLayout(){
    layout = new QVBoxLayout(this);
    boards_layout = new QHBoxLayout(this);
    button_layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Review:"));
    boards_layout->addWidget(_my_frame);
    layout->addLayout(boards_layout);
    button_layout = new QHBoxLayout();
    button_layout->addWidget(next_button);
    button_layout->addWidget(previous_button);
    button_layout->addWidget(back_main_button);
    layout->addLayout(button_layout);
}


void ReviewGame::onNextButtonClicked(){
    _controller->setNextMove();
}

void ReviewGame::onPreviousButtonClicked(){
    _controller->setPreviousMove();
}

void ReviewGame::onBackMainButtonClicked(){
    emit goToMainMenu();
}

ReviewGame::~ReviewGame(){}

BoardReviewFrame::~BoardReviewFrame(){}
