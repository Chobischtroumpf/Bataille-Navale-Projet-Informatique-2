#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <qt6/QtWidgets/qpushbutton.h>
#include <QRegularExpression>
#include <QEvent> // for QEvent
#include <QMouseEvent> // for QMouseEvent
#include <QEnterEvent> // for QEnterEvent
#include <QFocusEvent> // for QFocusEvent
#include <QPainter>
#include <QMovie>

#include "review_controller.hh"
#include "review_view.hh"
//#include "highlight_button.hh"

class Review : public QWidget{
    Q_OBJECT 
private:
    std::shared_ptr<ReviewController> _review_controller;
    QListWidget *game_list_view;
    QPushButton *r_game_button;
    QPushButton *back_main_button;
    QVBoxLayout *layout;
    QHBoxLayout *button_layout;

    /**
     * @brief Set up buttons for the user interface.
     */
    void setButtons();

    /**
     * @brief Set up the list view for displaying data.
     */
    void setListView();

    /**
     * @brief Set up the layout for the user interface.
     */
    void setLayout();
    
public:
    /** Constructor, Destructor */
    Review(std::shared_ptr<ReviewController> review_controller);
    ~Review();

private slots:
    /**
     * @brief Handle the click event of the "Review Game" button.
     */
    void onRGameButtonClicked();

    /**
     * @brief Handle the click event of the "Back to Main" button.
     */
    void onBackMainButtonClicked();

signals:
    /**
     * @brief Signal emitted to navigate to the game review page.
     *
     * @param selected_game The selected game to review.
     */
    void goToGameReview(std::string& selected_game);

    /**
     * @brief Signal emitted to navigate back to the main menu.
     */
    void goToMainMenu();
};


