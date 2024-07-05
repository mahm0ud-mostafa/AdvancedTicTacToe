#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void handleButtonClick();
    void on_loginButton_clicked();
    void on_signupButton_clicked();
    void on_registerButton_clicked();
    void on_playerVsPlayerButton_clicked();
    void on_playerVsAiButton_clicked();
    void on_backButtonGamePage_clicked();
    void on_backButtonSignupPage_clicked();
    void on_myAccountButton_clicked();
    void on_backButtonPage3_clicked();
    void on_selectXButton_clicked();
    void on_selectOButton_clicked();
    void on_backButtonPage4_clicked();
    void on_resetButton_clicked();

private:
    Ui::Widget *ui;
    bool isVsAI; // Flag to indicate if the game is Player vs AI

    // AI-related methods
    int minimax(int board[3][3], bool isMaximizing);
    QPoint findBestMove(int board[3][3]);
    bool isMovesLeft(int board[3][3]);
    int evaluateBoard(int board[3][3]);

    // Game-related methods
    int checkWin();
    bool isBoardFull();
    void resetBoard();
    QPushButton* getButton(int row, int col);
    void showWinnerMessage(int winResult); // New method to display the winner's message

    // Database related methods
    void initializeDatabase();
    bool registerUser(const QString &username, const QString &password);
    bool authenticateUser(const QString &username, const QString &password);

};

#endif // WIDGET_H
