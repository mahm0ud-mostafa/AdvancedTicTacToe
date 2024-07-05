# Advanced Tic Tac Toe

Advanced Tic Tac Toe is a C++/Qt-based version of the classic Tic Tac Toe game, featuring AI opponents, user authentication, and game history functionality. The project is designed to provide an enhanced user experience with multiple game modes and features.

## Features

- **Player vs Player**: Play against another human player.
- **Player vs AI**: Challenge the computer with two difficulty levels - Easy (random algorithm) and Hard (minmax algorithm).
- **User Authentication**: Register, login, and manage user accounts.
- **Forget Password**: Recover lost account passwords.
- **Game History**: View and replay past games.
- **Enhanced GUI**: User-friendly interface with improved visual elements.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/mahmoud-diasty/AdvancedTicTacToe.git
   cd AdvancedTicTacToe
   ```

2. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the application:
   ```bash
   ./AdvancedTicTacToe
   ```

## Usage

Upon starting the application, you will be presented with the main menu where you can:

- Register a new user
- Login with an existing account
- Start a new game
- View game history

During the game, you can choose to play against another player or the AI, with the option to select the difficulty level.

## Project Structure

- **main.cpp**: Entry point of the application.
- **widget.cpp**: Main game logic and UI handling.
- **widget.h**: Header file for `widget.cpp`.
- **widget.ui**: Qt Designer file for the main UI layout.
- **CMakeLists.txt**: CMake configuration file for building the project.

## Testing

The project includes a test bench for automated testing of the game's functionality.

```cpp
// Example test_bench.cpp
#include <gtest/gtest.h>
#include "widget.h"

TEST(AdvancedTicTacToeTest, PlayerXWins) {
    // Setup
    Widget game;
    game.resetBoard();

    // Simulate moves leading to Player X win
    game.makeMove(0, 0, 1); // X
    game.makeMove(0, 1, -1); // O
    game.makeMove(1, 0, 1); // X
    game.makeMove(1, 1, -1); // O
    game.makeMove(2, 0, 1); // X

    // Verify
    EXPECT_EQ(game.checkWin(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

## Contributing

We welcome contributions to enhance the features and improve the code quality. Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch:
    ```bash
    git checkout -b feature-branch
    ```
3. Make your changes and commit them:
    ```bash
    git commit -m "Description of your changes"
    ```
4. Push to the branch:
    ```bash
    git push origin feature-branch
    ```
5. Create a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to the Qt Project for providing an excellent framework for developing this application.
- Special thanks to all the contributors who have helped improve this project.

## Contact

If you have any questions or feedback, please reach out to mahmoud.mustafa302@eng-st.cu.edu.eg.
