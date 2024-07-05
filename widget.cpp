#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QRandomGenerator>

// Global variables
char current_player = 'X';
int board[3][3] = {0}; // 0 = empty, 1 = X, -1 = O
bool isVsAI = false;
QString aiDifficulty = "hard"; // Default AI difficulty level

// Constructor
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), isVsAI(false), currentUser(""), replayTimer(new QTimer(this)), replayIndex(0)
{
    ui->setupUi(this);
    initializeDatabase();
    setupConnections(); // Call setupConnections to initialize connections
    ui->stackedWidget->setCurrentIndex(0);

    // Set hard difficulty as the default
    ui->hardRadioButton->setChecked(true); // Ensure hard level is chosen by default
    aiDifficulty = "hard"; // Set the default value of aiDifficulty

}

Widget::~Widget()
{
    delete ui;
}

// Setup connections for the UI elements
void Widget::setupConnections()
{
    // Login Page
    connect(ui->loginButton, &QPushButton::clicked, this, &Widget::on_loginButton_clicked);
    connect(ui->signupLoginPageButton, &QPushButton::clicked, this, &Widget::on_signupLoginPageButton_clicked);
    connect(ui->showLoginPasswordCheckBox, &QCheckBox::toggled, this, &Widget::on_showLoginPasswordCheckBox_toggled);
    connect(ui->noWorriesButton, &QPushButton::clicked, this, &Widget::on_noWorriesButton_clicked);

    // Sign-Up Page
    connect(ui->registerButton, &QPushButton::clicked, this, &Widget::on_registerButton_clicked);
    connect(ui->showSignupPasswordCheckBox, &QCheckBox::toggled, this, &Widget::on_showSignupPasswordCheckBox_toggled);
    connect(ui->loginButtonSignupPage, &QPushButton::clicked, this, &Widget::on_loginButtonSignupPage_clicked);

    // Main Page
    connect(ui->playerVsAiButton, &QPushButton::clicked, this, &Widget::on_playerVsAiButton_clicked);
    connect(ui->playerVsPlayerButton, &QPushButton::clicked, this, &Widget::on_playerVsPlayerButton_clicked);
    connect(ui->myAccountButton, &QPushButton::clicked, this, &Widget::on_myAccountButton_clicked);
    connect(ui->myGameHistoryButton, &QPushButton::clicked, this, &Widget::on_myGameHistoryButton_clicked);

    // Password Reset Page
    connect(ui->resetPasswordButton, &QPushButton::clicked, this, &Widget::on_resetPasswordButton_clicked);
    connect(ui->showResetNewPasswordCheckBox, &QCheckBox::toggled, this, &Widget::on_showResetNewPasswordCheckBox_toggled);
    connect(ui->backButtonReset, &QPushButton::clicked, this, &Widget::on_backButtonReset_clicked);

    // Personal Info Page
    connect(ui->backButtonAccount, &QPushButton::clicked, this, &Widget::on_backButtonAccount_clicked);
    connect(ui->changePasswordButton, &QPushButton::clicked, this, &Widget::on_changePasswordButton_clicked);
    connect(ui->logoutButtonAccount, &QPushButton::clicked, this, &Widget::on_logoutButtonAccount_clicked);

    // Game Board Page
    connect(ui->pushButton_00, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_01, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_02, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_20, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_22, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->resetGameboardButton, &QPushButton::clicked, this, &Widget::on_resetGameboardButton_clicked);
    connect(ui->backButtonGamePage, &QPushButton::clicked, this, &Widget::on_backButtonGamePage_clicked);

    // Game History Page
    connect(ui->replayGameButton, &QPushButton::clicked, this, &Widget::on_replayGameButton_clicked);
    connect(ui->deleteGameButton, &QPushButton::clicked, this, &Widget::on_deleteGameButton_clicked);
    connect(ui->backButtonHistory, &QPushButton::clicked, this, &Widget::on_backButtonHistory_clicked);

    // Connect the timer to the replayNextMove slot
    connect(replayTimer, &QTimer::timeout, this, &Widget::replayNextMove);

    // Connect the difficulty selection radio buttons
    connect(ui->easyRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked) {
            aiDifficulty = "easy";
        }
    });
    connect(ui->hardRadioButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked) {
            aiDifficulty = "hard";
        }
    });
}


// Handles the login button click event
void Widget::on_loginButton_clicked() {
    QString username = ui->loginUsernameLineEdit->text();
    QString password = ui->loginPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
        return;
    }

    if (authenticateUser(username, password)) {
        QString welcomeMessage = "Welcome!";
        QSqlQuery query;
        query.prepare("SELECT firstName, lastName FROM users WHERE username = :username");
        query.bindValue(":username", username);
        if (query.exec() && query.next()) {
            QString firstName = query.value(0).toString();
            QString lastName = query.value(1).toString();
            if (!firstName.isEmpty() || !lastName.isEmpty()) {
                welcomeMessage = "Welcome, " + firstName + " " + lastName + "!";
            }
        }
        QMessageBox::information(this, "Login Successful", welcomeMessage);
        ui->stackedWidget->setCurrentIndex(1); // Proceed to the main page
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

// Handles the sign-up button click from the login page
void Widget::on_signupLoginPageButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2); // Go to the sign-up page
}

// Handles the registration button click event
void Widget::on_registerButton_clicked() {
    QString username = ui->signupUsernameLineEdit->text();
    QString password = ui->signupPasswordLineEdit->text();
    QString firstName = ui->signupFirstNameLineEdit->text();
    QString lastName = ui->signupLastNameLineEdit->text();
    QString securityQuestion = ui->securityQuestionLineEdit->text();
    QString securityAnswer = ui->securityAnswerLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Failed", "Please fill in all required fields.");
        return;
    }

    if (registerUser(username, password, firstName, lastName, securityQuestion, securityAnswer)) {
        QMessageBox::information(this, "Signup Successful", "You have been registered.");
        ui->stackedWidget->setCurrentIndex(0); // Go back to the login page
    } else {
        QMessageBox::warning(this, "Signup Failed", "Could not register user.");
    }
}

// Handle the login button click on the sign-up page
void Widget::on_loginButtonSignupPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); // Go back to the login page
}

// Handles the "Forgot Password" button click event
void Widget::on_noWorriesButton_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Go to the Forgot Password page
}

// Handles the reset password button click event
void Widget::on_resetPasswordButton_clicked() {
    QString username = ui->resetUsernameLineEdit->text();
    QString newPassword = ui->resetNewPasswordLineEdit->text();
    QString confirmPassword = ui->resetConfirmPasswordLineEdit->text();
    QString securityQuestion = ui->resetSecurityQuestionLineEdit->text();
    QString securityAnswer = ui->resetSecurityAnswerLineEdit->text();

    if (username.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty() || securityQuestion.isEmpty() || securityAnswer.isEmpty()) {
        QMessageBox::warning(this, "Reset Password Failed", "Please fill in all required fields.");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Reset Password Failed", "Passwords do not match.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND securityQuestion = :securityQuestion AND securityAnswer = :securityAnswer");
    query.bindValue(":username", username);
    query.bindValue(":securityQuestion", securityQuestion);
    query.bindValue(":securityAnswer", securityAnswer);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Reset Password Failed", "Invalid username or security information.");
        return;
    }

    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256).toHex());
    query.bindValue(":username", username);

    if (query.exec()) {
        QMessageBox::information(this, "Reset Password Successful", "Your password has been reset.");
        ui->stackedWidget->setCurrentIndex(0); // Go back to the login page
    } else {
        QMessageBox::warning(this, "Reset Password Failed", "Could not reset password.");
    }
}

// Toggles password visibility
void Widget::togglePasswordVisibility(QLineEdit* lineEdit, bool visible) {
    if (visible) {
        lineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        lineEdit->setEchoMode(QLineEdit::Password);
    }
}

// Handles the "Show Password" CheckBox toggle event on the login page
void Widget::on_showLoginPasswordCheckBox_toggled(bool checked) {
    togglePasswordVisibility(ui->loginPasswordLineEdit, checked);
}

// Handles the "Show Password" CheckBox toggle event on the sign-up page
void Widget::on_showSignupPasswordCheckBox_toggled(bool checked) {
    togglePasswordVisibility(ui->signupPasswordLineEdit, checked);
}

// Handles the "Show Password" CheckBox toggle event on the reset password page
void Widget::on_showResetNewPasswordCheckBox_toggled(bool checked) {
    togglePasswordVisibility(ui->resetNewPasswordLineEdit, checked);
    togglePasswordVisibility(ui->resetConfirmPasswordLineEdit, checked);
}

// Handles the back button click event on the reset password page
void Widget::on_backButtonReset_clicked() {
    ui->stackedWidget->setCurrentIndex(0); // Go back to the login page
}

// Handles the back button click event on the personal info page
void Widget::on_backButtonAccount_clicked() {
    ui->stackedWidget->setCurrentIndex(1); // Go back to the main page
}

// Handles the change password button click event on the personal info page
void Widget::on_changePasswordButton_clicked() {
    ui->stackedWidget->setCurrentIndex(3); // Go to the password reset page
}

// Handles the logout button click event on the personal info page
void Widget::on_logoutButtonAccount_clicked() {
    currentUser.clear(); // Clear the current user
    ui->stackedWidget->setCurrentIndex(0); // Go back to the login page
}

// Handles the reset gameboard button click event
void Widget::on_resetGameboardButton_clicked() {
    resetBoard();
    ui->stackedWidget->setCurrentIndex(5); // Go to the game board page
}

// Handles the back button click event on the game board page
void Widget::on_backButtonGamePage_clicked() {
    enableGameboard();
    ui->stackedWidget->setCurrentIndex(1); // Go back to the main page
}

// Handles the "Player vs AI" button click event
void Widget::on_playerVsAiButton_clicked() {
    isVsAI = true; // Set flag to true for Player vs AI mode
    current_player = 'X';
    resetBoard();
    enableGameboard(); // Re-enable the gameboard buttons
    ui->stackedWidget->setCurrentIndex(5); // Go to the game board page directly
    ui->gameStatusLabel->setText("Your Turn!!"); // Display "Your Turn!!" for the user
}

// Handles the "Player vs Player" button click event
void Widget::on_playerVsPlayerButton_clicked() {
    isVsAI = false; // Set flag to false for Player vs Player mode
    resetBoard();
    enableGameboard(); // Re-enable the gameboard buttons
    ui->stackedWidget->setCurrentIndex(5); // Go to the game board page
    ui->gameStatusLabel->setText("Turn of Player: " + QString(current_player));
}

// Handles button click events on the game board
void Widget::handleButtonClick() {
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

        moveHistory.append(QString::number(row) + ":" + QString::number(col) + ":" + current_player); // Record move

        if (current_player == 'X') {
            board[row][col] = 1;
            current_player = 'O';
            ui->gameStatusLabel->setText(isVsAI ? "AI Turn" : "Turn of Player: " + QString(current_player));
        } else {
            board[row][col] = -1;
            current_player = 'X';
            ui->gameStatusLabel->setText(isVsAI ? "Your Turn!!" : "Turn of Player: " + QString(current_player));
        }

        int winResult = checkWin();
        if (winResult != 0) {
            showWinnerMessage(winResult);
            saveGameHistory(currentUser, isVsAI ? "AI" : "Player O", winResult == 1 ? "Player X wins!" : "Player O wins!", moveHistory);
            resetBoard();
        } else if (isBoardFull()) {
            QMessageBox::information(this, "Game Over", "Game over. It's a draw!");
            saveGameHistory(currentUser, isVsAI ? "AI" : "Player O", "Draw", moveHistory);
            resetBoard();
        } else {
            if (isVsAI && current_player == 'O') {
                QPoint bestMove;
                if (aiDifficulty == "easy") {
                    bestMove = findRandomMove(board);
                } else {
                    bestMove = findBestMove(board);
                }
                if (bestMove.x() != -1 && bestMove.y() != -1) {
                    QPushButton *aiButton = getButton(bestMove.x(), bestMove.y());
                    aiButton->setText(QString(current_player));
                    board[bestMove.x()][bestMove.y()] = -1;
                    moveHistory.append(QString::number(bestMove.x()) + ":" + QString::number(bestMove.y()) + ":" + current_player); // Record AI move
                    current_player = 'X';
                    ui->gameStatusLabel->setText("Your Turn!!");

                    winResult = checkWin();
                    if (winResult != 0) {
                        showWinnerMessage(winResult);
                        saveGameHistory(currentUser, "AI", winResult == 1 ? "Player X wins!" : "AI wins!", moveHistory);
                        resetBoard();
                    }
                }
            }
        }
    }
}

// Finds a random move for the AI in easy mode
QPoint Widget::findRandomMove(int board[3][3]) {
    QVector<QPoint> availableMoves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                availableMoves.append(QPoint(i, j));
            }
        }
    }
    if (availableMoves.isEmpty()) {
        return QPoint(-1, -1);
    } else {
        int index = QRandomGenerator::global()->bounded(availableMoves.size());
        return availableMoves[index];
    }
}

// Evaluates the board for the AI
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

// Minimax algorithm for the AI
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

// Finds the best move for the AI
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

// Replays the next move in the game history
void Widget::replayNextMove() {
    if (replayIndex < replayMoves.size()) {
        QStringList pos = replayMoves[replayIndex].split(":");
        int row = pos[0].toInt();
        int col = pos[1].toInt();
        QString player = pos[2];
        QPushButton *button = getButton(row, col);
        button->setText(player);
        board[row][col] = (player == "X") ? 1 : -1;

        // Update gameStatusLabel correctly during the replay
        if (player == "X") {
            ui->gameStatusLabel->setText(isVsAI ? "Game Replay: Your Turn!!" : "Game Replay: Player X turn");
        } else {
            ui->gameStatusLabel->setText(isVsAI ? "Game Replay: AI Turn" : "Game Replay: Player O turn");
        }

        replayIndex++;
    } else {
        // Stop the replay and keep the gameboard disabled
        replayTimer->stop();
    }
}

// Handles the "Replay Game" button click event
void Widget::on_replayGameButton_clicked() {
    QListWidgetItem *selectedItem = ui->gameHistoryListWidget->currentItem();
    if (selectedItem) {
        int gameId = selectedItem->data(Qt::UserRole).toInt();
        QSqlQuery query;
        query.prepare("SELECT player1, player2, moves FROM game_history WHERE id = :gameId");
        query.bindValue(":gameId", gameId);
        if (query.exec() && query.next()) {
            QString player1 = query.value(0).toString();
            QString player2 = query.value(1).toString();
            replayMoves = query.value(2).toString().split(",");

            resetBoard();
            current_player = 'X';
            disableGameboard();
            replayIndex = 0;
            replayTimer->start(1000); // Set the interval to 1 second
            ui->stackedWidget->setCurrentIndex(5); // Go to the game board page
            ui->gameStatusLabel->setText(isVsAI ? "Game Replay: Your Turn!!" : "Game Replay: Player X turn");
        } else {
            qDebug() << "Error: failed to load game details" << query.lastError();
        }
    } else {
        QMessageBox::warning(this, "Replay Game", "Please select a game to replay.");
    }
}

// Disables the gameboard buttons
void Widget::disableGameboard() {
    ui->pushButton_00->setDisabled(true);
    ui->pushButton_01->setDisabled(true);
    ui->pushButton_02->setDisabled(true);
    ui->pushButton_10->setDisabled(true);
    ui->pushButton_11->setDisabled(true);
    ui->pushButton_12->setDisabled(true);
    ui->pushButton_20->setDisabled(true);
    ui->pushButton_21->setDisabled(true);
    ui->pushButton_22->setDisabled(true);
}

// Enables the gameboard buttons
void Widget::enableGameboard() {
    ui->pushButton_00->setEnabled(true);
    ui->pushButton_01->setEnabled(true);
    ui->pushButton_02->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_20->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
}

// Resets the game board
void Widget::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    current_player = 'X';
    moveHistory.clear(); // Clear moveHistory
    ui->gameStatusLabel->setText(isVsAI ? "Your Turn!!" : "Turn of Player: " + QString(current_player));
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

// Handles the "My Account" button click event
void Widget::on_myAccountButton_clicked() {
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName, username FROM users WHERE username = :username");
    query.bindValue(":username", currentUser); // Use the stored current user
    if (query.exec() && query.next()) {
        QString firstName = query.value(0).toString();
        QString lastName = query.value(1).toString();
        QString username = query.value(2).toString();

        ui->accountFirstNameLabel->setText(firstName);
        ui->accountLastNameLabel->setText(lastName);
        ui->accountUsernameLabel->setText(username);
    } else {
        qDebug() << "Query failed!" << query.lastError();
    }
    ui->stackedWidget->setCurrentIndex(4); // Go to the My Account page
}

// Handles the "My Game History" button click event
void Widget::on_myGameHistoryButton_clicked() {
    ui->stackedWidget->setCurrentIndex(6); // Go to the game history page
    loadGameHistory();
}

// Loads game history from the database
void Widget::loadGameHistory() {
    ui->gameHistoryListWidget->clear(); // Clear the list widget before loading history
    QSqlQuery query;
    query.prepare("SELECT id, player1, player2, result, moves, timestamp FROM game_history WHERE player1 = :currentUser OR player2 = :currentUser");
    query.bindValue(":currentUser", currentUser);
    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString player1 = query.value(1).toString();
            QString player2 = query.value(2).toString();
            QString result = query.value(3).toString();
            QString moves = query.value(4).toString();
            QString timestamp = query.value(5).toString();

            QString historyEntry = QString("Player 1: %1\nPlayer 2: %2\nResult: %3\nMoves: %4\nDate: %5")
                                       .arg(player1)
                                       .arg(player2)
                                       .arg(result)
                                       .arg(moves)
                                       .arg(timestamp);
            QListWidgetItem *item = new QListWidgetItem(historyEntry, ui->gameHistoryListWidget);
            item->setData(Qt::UserRole, id); // Store the game ID for replaying
        }
        if (ui->gameHistoryListWidget->count() == 0) {
            ui->gameHistoryListWidget->addItem("No game history available.");
        }
    } else {
        qDebug() << "Error: failed to load game history" << query.lastError();
        ui->gameHistoryListWidget->addItem("Failed to load game history.");
    }
}

// Handles the "Delete Game" button click event
void Widget::on_deleteGameButton_clicked() {
    QListWidgetItem *selectedItem = ui->gameHistoryListWidget->currentItem();
    if (selectedItem) {
        int gameId = selectedItem->data(Qt::UserRole).toInt();
        QSqlQuery query;
        query.prepare("DELETE FROM game_history WHERE id = :gameId");
        query.bindValue(":gameId", gameId);
        if (query.exec()) {
            delete selectedItem; // Remove from UI
            QMessageBox::information(this, "Delete Game", "Game history deleted successfully.");
        } else {
            qDebug() << "Error: failed to delete game history" << query.lastError();
            QMessageBox::warning(this, "Delete Game", "Failed to delete game history.");
        }
    } else {
        QMessageBox::warning(this, "Delete Game", "Please select a game to delete.");
    }
}

// Handles the back button click event on the game history page
void Widget::on_backButtonHistory_clicked() {
    ui->stackedWidget->setCurrentIndex(1); // Go back to the main page
}

// Initializes the database
void Widget::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";

        QSqlQuery query;
        // Create users table if it doesn't exist
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "username TEXT NOT NULL,"
                   "password TEXT NOT NULL,"
                   "firstName TEXT,"
                   "lastName TEXT,"
                   "securityQuestion TEXT,"
                   "securityAnswer TEXT"
                   ")");

        // Create game_history table if it doesn't exist
        query.exec("CREATE TABLE IF NOT EXISTS game_history ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "player1 TEXT,"
                   "player2 TEXT,"
                   "result TEXT,"
                   "moves TEXT,"
                   "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                   ")");
    }
}

// Registers a new user
bool Widget::registerUser(const QString &username, const QString &password, const QString &firstName, const QString &lastName, const QString &securityQuestion, const QString &securityAnswer) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, firstName, lastName, securityQuestion, securityAnswer) VALUES (:username, :password, :firstName, :lastName, :securityQuestion, :securityAnswer)");
    query.bindValue(":username", username);
    query.bindValue(":password", QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()); // Hashing password
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":securityQuestion", securityQuestion);
    query.bindValue(":securityAnswer", securityAnswer);

    if (!query.exec()) {
        qDebug() << "Error: failed to insert new user" << query.lastError();
        return false;
    }
    return true;
}

// Authenticates an existing user
bool Widget::authenticateUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()); // Hashing and comparing

    if (!query.exec()) {
        qDebug() << "Error: failed to authenticate user" << query.lastError();
        return false;
    }

    if (query.next()) {
        currentUser = username; // Store the current user
        qDebug() << "User authenticated: " << currentUser;
        return true;
    }
    return false;
}

// Checks if there are any moves left on the board
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

// Checks if there is a winner on the board
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

// Checks if the board is full
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

// Saves game history to the database
void Widget::saveGameHistory(const QString &player1, const QString &player2, const QString &result, const QStringList &moves) {
    QSqlQuery query;
    query.prepare("INSERT INTO game_history (player1, player2, result, moves) VALUES (:player1, :player2, :result, :moves)");
    query.bindValue(":player1", player1);
    query.bindValue(":player2", player2);
    query.bindValue(":result", result);
    query.bindValue(":moves", moves.join(",")); // Save moves as a comma-separated string
    if (!query.exec()) {
        qDebug() << "Error: failed to insert game history" << query.lastError();
    } else {
        qDebug() << "Game history saved successfully: Player 1: " << player1 << ", Player 2: " << player2 << ", Result: " << result << ", Moves: " << moves.join(" -> ");
    }
}

// Displays a winner message and resets the board
void Widget::showWinnerMessage(int winResult) {
    QString result;
    if (winResult == 1) {
        result = "Player X wins!";
        QMessageBox::information(this, "Congratulations!", result);
    } else if (winResult == -1) {
        if (isVsAI) {
            result = "AI wins! Better luck next time!";
            QMessageBox::information(this, "AI wins", result);
        } else {
            result = "Player O wins!";
            QMessageBox::information(this, "Congratulations!", result);
        }
    }

    // Save game history after showing the winner message
    saveGameHistory(currentUser, isVsAI ? "AI" : "Player O", result, moveHistory); // Update with actual player names and moves

    resetBoard();
}

// Retrieves the button based on the row and column
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
