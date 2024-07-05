#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

char current_player = 'X';
int board[3][3] = {0}; // 0 = empty, 1 = X, -1 = O
bool isVsAI = false; // Flag to indicate if the game is Player vs AI

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    initializeDatabase();

    // Set the initial page to the login page
    ui->stackedWidget->setCurrentIndex(0);  // Replace 0 with the index of your desired initial page

    // Connect the game board buttons
    connect(ui->pushButton_00, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_01, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_02, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_20, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_22, &QPushButton::clicked, this, &Widget::handleButtonClick);

    // Connect the other buttons
    connect(ui->loginButton, &QPushButton::clicked, this, &Widget::on_loginButton_clicked);
    connect(ui->signupButton, &QPushButton::clicked, this, &Widget::on_signupButton_clicked);
    connect(ui->playerVsPlayerButton, &QPushButton::clicked, this, &Widget::on_playerVsPlayerButton_clicked);
    connect(ui->playerVsAiButton, &QPushButton::clicked, this, &Widget::on_playerVsAiButton_clicked);
    connect(ui->backButtonGamePage, &QPushButton::clicked, this, &Widget::on_backButtonGamePage_clicked);
    connect(ui->backButtonSignupPage, &QPushButton::clicked, this, &Widget::on_backButtonSignupPage_clicked);
    connect(ui->myAccountButton, &QPushButton::clicked, this, &Widget::on_myAccountButton_clicked);
    connect(ui->backButtonPage3, &QPushButton::clicked, this, &Widget::on_backButtonPage3_clicked);
    connect(ui->selectXButton, &QPushButton::clicked, this, &Widget::on_selectXButton_clicked);
    connect(ui->selectOButton, &QPushButton::clicked, this, &Widget::on_selectOButton_clicked);
    connect(ui->backButtonPage4, &QPushButton::clicked, this, &Widget::on_backButtonPage4_clicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &Widget::on_resetButton_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleButtonClick()
{
    QPushButton *Clicked_button = qobject_cast<QPushButton *>(sender());
    if (Clicked_button->text().isEmpty()) {
        Clicked_button->setText(QString(current_player));
        int row, col;
        if (Clicked_button == ui->pushButton_00) { row = 0; col = 0; }
        else if (Clicked_button == ui->pushButton_01) { row = 0; col = 1; }
        else if (Clicked_button == ui->pushButton_02) { row = 0; col = 2; }
        else if (Clicked_button == ui->pushButton_10) { row = 1; col = 0; }
        else if (Clicked_button == ui->pushButton_11) { row = 1; col = 1; }
        else if (Clicked_button == ui->pushButton_12) { row = 1; col = 2; }
        else if (Clicked_button == ui->pushButton_20) { row = 2; col = 0; }
        else if (Clicked_button == ui->pushButton_21) { row = 2; col = 1; }
        else if (Clicked_button == ui->pushButton_22) { row = 2; col = 2; }

        if (current_player == 'X') {
            board[row][col] = 1;
            current_player = 'O';
            ui->whoseTurnLabel->setText(isVsAI ? "AI Turn" : "Turn of Player: "+QString(current_player));
        } else {
            board[row][col] = -1;
            current_player = 'X';
            ui->whoseTurnLabel->setText(isVsAI ? "Your Turn!!" : "Turn of Player: "+QString(current_player));
        }

        int winResult = checkWin();
        if (winResult != 0) {
            showWinnerMessage(winResult);
            resetBoard();
        } else if (isBoardFull()) {
            QMessageBox::information(this, "Game Over", "Game over. It's a draw!");
            resetBoard();
        } else {
            if (isVsAI && current_player == 'O') {
                QPoint bestMove = findBestMove(board);
                if (bestMove.x() != -1 && bestMove.y() != -1) {
                    QPushButton *aiButton = getButton(bestMove.x(), bestMove.y());
                    aiButton->setText(QString(current_player));
                    board[bestMove.x()][bestMove.y()] = -1;
                    current_player = 'X';
                    ui->whoseTurnLabel->setText("Your Turn!!");

                    winResult = checkWin();
                    if (winResult != 0) {
                        showWinnerMessage(winResult);
                        resetBoard();
                    }
                }
            }
        }
    }
}

void Widget::showWinnerMessage(int winResult)
{
    if (winResult == 1) {
        QMessageBox::information(this, "Congratulations!", "Player X wins!");
    } else if (winResult == -1) {
        if (isVsAI) {
            QMessageBox::information(this, "AI wins", "AI wins! Better luck next time!");
        } else {
            QMessageBox::information(this, "Congratulations!", "Player O wins!");
        }
    }
}

QPushButton* Widget::getButton(int row, int col) {
    if (row == 0 && col == 0) return ui->pushButton_00;
    if (row == 0 && col == 1) return ui->pushButton_01;
    if (row == 0 && col == 2) return ui->pushButton_02;
    if (row == 1 && col == 0) return ui->pushButton_10;
    if (row == 1 && col == 1) return ui->pushButton_11;
    if (row == 1 && col == 2) return ui->pushButton_12;
    if (row == 2 && col == 0) return ui->pushButton_20;
    if (row == 2 && col == 1) return ui->pushButton_21;
    if (row == 2 && col == 2) return ui->pushButton_22;
    return nullptr;
}

int Widget::checkWin() {
    for (int row = 0; row < 3; ++row) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2] && board[row][0] != 0) {
            return board[row][0];
        }
    }
    for (int col = 0; col < 3; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != 0) {
            return board[0][col];
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
        return board[0][2];
    }
    return 0;
}

bool Widget::isBoardFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

void Widget::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    current_player = 'X';
    ui->whoseTurnLabel->setText(isVsAI ? "Your Turn!!" : "Turn of Player: "+QString(current_player));
    ui->pushButton_00->setText("");
    ui->pushButton_01->setText("");
    ui->pushButton_02->setText("");
    ui->pushButton_10->setText("");
    ui->pushButton_11->setText("");
    ui->pushButton_12->setText("");
    ui->pushButton_20->setText("");
    ui->pushButton_21->setText("");
    ui->pushButton_22->setText("");
}

int Widget::evaluateBoard(int board[3][3]) {
    // Evaluation logic for the AI to determine the score
    // Return +10 if AI wins, -10 if player wins, 0 otherwise
    // Check rows, columns, and diagonals for win conditions
    for (int row = 0; row < 3; ++row) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == -1) return +10;
            else if (board[row][0] == 1) return -10;
        }
    }
    for (int col = 0; col < 3; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == -1) return +10;
            else if (board[0][col] == 1) return -10;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == -1) return +10;
        else if (board[0][0] == 1) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == -1) return +10;
        else if (board[0][2] == 1) return -10;
    }
    return 0;
}

bool Widget::isMovesLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

int Widget::minimax(int board[3][3], bool isMaximizing) {
    int score = evaluateBoard(board);

    if (score == 10 || score == -10) {
        return score;
    }

    if (!isMovesLeft(board)) {
        return 0;
    }

    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = -1;
                    best = std::max(best, minimax(board, false));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 1;
                    best = std::min(best, minimax(board, true));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

QPoint Widget::findBestMove(int board[3][3]) {
    int bestVal = -1000;
    QPoint bestMove(-1, -1);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                board[i][j] = -1;
                int moveVal = minimax(board, false);
                board[i][j] = 0;

                if (moveVal > bestVal) {
                    bestMove = QPoint(i, j);
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

void Widget::on_playerVsPlayerButton_clicked() {
    isVsAI = false; // Set flag to false for Player vs Player mode
    resetBoard();
    ui->stackedWidget->setCurrentIndex(5);
    ui->whoseTurnLabel->setText("Turn of Player: "+QString(current_player));
}

void Widget::on_playerVsAiButton_clicked() {
    isVsAI = true; // Set flag to true for Player vs AI mode
    current_player = 'X';
    resetBoard();
    ui->stackedWidget->setCurrentIndex(5);  // Go to the game board page directly
    ui->whoseTurnLabel->setText("Your Turn!!");  // Display "Your Turn!!" for the user
}

void Widget::on_backButtonGamePage_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_backButtonSignupPage_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_myAccountButton_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::on_backButtonPage3_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_selectXButton_clicked() {
    current_player = 'X';
    ui->whoseTurnLabel->setText("Your Turn!!");
    ui->stackedWidget->setCurrentIndex(5);
}

void Widget::on_selectOButton_clicked() {
    current_player = 'O';
    ui->whoseTurnLabel->setText("Your Turn!!");
    ui->stackedWidget->setCurrentIndex(5);
}

void Widget::on_backButtonPage4_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_resetButton_clicked() {
    resetBoard();
    ui->stackedWidget->setCurrentIndex(5);
}

/*_________DATABASE MANAGEMENT SYSTEM_________*/

void Widget::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";

        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "username TEXT NOT NULL,"
                   "password TEXT NOT NULL"
                   ")");
    }
}

bool Widget::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // Consider hashing the password

    if (!query.exec()) {
        qDebug() << "Error: failed to insert new user" << query.lastError();
        return false;
    }
    return true;
}

bool Widget::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // Consider hashing and comparing

    if (!query.exec()) {
        qDebug() << "Error: failed to authenticate user" << query.lastError();
        return false;
    }

    if (query.next()) {
        return true;
    }
    return false;
}


void Widget::on_signupButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

// void Widget::on_signupButton_clicked()
// {
//     QString username = ui->signupUsernameLineEdit->text();
//     QString password = ui->signupPasswordLineEdit->text();

//     // Debugging: Print username and password to the console
//     qDebug() << "Signup username:" << username;
//     qDebug() << "Signup password:" << password;

//     if (username.isEmpty() || password.isEmpty()) {
//         QMessageBox::warning(this, "Signup Failed", "Please enter both username and password.");
//         return;
//     }

//     if (registerUser(username, password)) {
//         QMessageBox::information(this, "Signup Successful", "You have been registered.");
//         ui->stackedWidget->setCurrentIndex(0); // Go back to login page
//     } else {
//         QMessageBox::warning(this, "Signup Failed", "Could not register user.");
//     }
// }


void Widget::on_loginButton_clicked()
{
    QString username = ui->loginUsernameLineEdit->text();
    QString password = ui->loginPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
        return;
    }

    if (authenticateUser(username, password)) {
        QMessageBox::information(this, "Login Successful", "Welcome!");
        ui->stackedWidget->setCurrentIndex(1); // Proceed to the next page
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void Widget::on_registerButton_clicked()
{
    QString username = ui->signupUsernameLineEdit->text();
    QString password = ui->signupPasswordLineEdit->text();

    // Debugging: Print username and password to the console
    qDebug() << "Signup username:" << username;
    qDebug() << "Signup password:" << password;

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Failed", "Please enter both username and password.");
        return;
    }

    if (registerUser(username, password)) {
        QMessageBox::information(this, "Signup Successful", "You have been registered.");
        ui->stackedWidget->setCurrentIndex(0); // Go back to login page
    } else {
        QMessageBox::warning(this, "Signup Failed", "Could not register user.");
    }
}
