#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>
#include <QPushButton>
#include <QThread>
#include <QLabel>
#include <vector>
#include <optional>

#include "board_coordinates.hh"
#include "review_game_controller.hh"
#include "gui.hh"
#include "local_board_review.hh"

class ReviewGame;

class BoardReviewFrame: public QFrame{
    Q_OBJECT

    private:
        std::shared_ptr<LocalBoardReview> _board;
        bool _my_side;
        BoardCoordinates _last_hovered = BoardCoordinates(0, 0);
        ReviewGame *_parent;

    protected:
        /**
         * @brief Draw a cell on the canvas.
         * 
         * @param painter The QPainter object used for painting.
         * @param x The x-coordinate of the cell.
         * @param y The y-coordinate of the cell.
         * @param cell The type of the cell to be drawn.
         */
        void drawCell(QPainter &painter, int x, int y, CellType cell);

        /**
         * @brief Override of the paint event of QWidget for custom painting.
         *
         * @param event The paint event object.
         */
        void paintEvent(QPaintEvent *event) override;

        /**
         * @brief Handle mouse press events.
         *
         * @param event The mouse press event object.
         */
        void mousePressEvent(QMouseEvent *event) override;

        /**
         * @brief Handle mouse move events.
         *
         * @param event The mouse move event object.
         */
        void mouseMoveEvent(QMouseEvent *event) override;

        /**
         * @brief Handle key press events.
         *
         * @param event The key press event object.
         */
        void keyPressEvent(QKeyEvent *event) override;

    public:
        /** Constructor, Destructor */
        BoardReviewFrame(ReviewGame *parent,std::shared_ptr<LocalBoardReview> board, bool my_side=true);
        ~BoardReviewFrame();

        /**
         * @brief Get the color brush for a specific cell type.
         *
         * @param cell The type of the cell.
         * @return The QBrush representing the color of the cell.
         */
        QBrush getTileColor(CellType cell);
};


class ReviewGame : public QWidget{
    Q_OBJECT

    private:
        std::shared_ptr<LocalBoardReview> _board;
        std::shared_ptr<ReviewGameController> _controller;
        bool _commander_mode;
        BoardReviewFrame *_my_frame;
        //BoardReviewFrame *_their_frame;
        QVBoxLayout *layout;
        QHBoxLayout *boards_layout;
        QHBoxLayout *footer_layout;
        QHBoxLayout *button_layout;
        QPushButton *next_button;
        QPushButton *previous_button;
        QPushButton *back_main_button;

        /**
         * @brief Set up buttons for the user interface.
         */
        void setButtons();

        /**
         * @brief Set up the layout for the user interface.
         */
        void setLayout();

    public:
        /** Constructor, Destructor */
        ReviewGame(std::shared_ptr<LocalBoardReview> board, std::shared_ptr<ReviewGameController> controller, bool my_side=true);
        ~ReviewGame();

    private slots:

        /**
         * @brief Handle the click event of the "Next" button.
         */
        void onNextButtonClicked();

        /**
         * @brief Handle the click event of the "Previous" button.
         */
        void onPreviousButtonClicked();

        /**
         * @brief Handle the click event of the "Back to Main" button.
         */
        void onBackMainButtonClicked();

    
    signals:
        /**
         * @brief Signal emitted when the application needs to navigate back to the main menu.
         */
        void goToMainMenu();

};

