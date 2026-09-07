#include <iostream>
#include <limits>
#include "TicTacToe.h"

static char board[3][3];
static int movesCount;

static void initBoard() {
    movesCount = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '1' + i * 3 + j;
}

static void displayBoard() {
    std::cout << "\n";
    std::cout << "       Tic Tac Toe\n";
    std::cout << "   -------------\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "   |";
        for (int j = 0; j < 3; j++) {
            std::cout << " " << board[i][j] << " |";
        }
        std::cout << "\n   -------------\n";
    }
    std::cout << "\n";
}

static bool checkWin(char player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;
    return false;
}

static bool checkDraw() {
    return movesCount == 9;
}

static bool makeMove(int pos, char player) {
    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;
    if (pos < 1 || pos > 9 || board[row][col] == 'X' || board[row][col] == 'O')
        return false;
    board[row][col] = player;
    movesCount++;
    return true;
}

static int getAIMove() {
    char ai = 'O';
    char human = 'X';

    for (int i = 1; i <= 9; i++) {
        int r = (i - 1) / 3, c = (i - 1) % 3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char orig = board[r][c];
            board[r][c] = ai;
            if (checkWin(ai)) { board[r][c] = orig; return i; }
            board[r][c] = orig;
        }
    }

    for (int i = 1; i <= 9; i++) {
        int r = (i - 1) / 3, c = (i - 1) % 3;
        if (board[r][c] != 'X' && board[r][c] != 'O') {
            char orig = board[r][c];
            board[r][c] = human;
            if (checkWin(human)) { board[r][c] = orig; return i; }
            board[r][c] = orig;
        }
    }

    if (board[1][1] != 'X' && board[1][1] != 'O') return 5;

    int corners[] = {1, 3, 7, 9};
    for (int i = 0; i < 4; i++) {
        int r = (corners[i] - 1) / 3, c = (corners[i] - 1) % 3;
        if (board[r][c] != 'X' && board[r][c] != 'O') return corners[i];
    }

    int edges[] = {2, 4, 6, 8};
    for (int i = 0; i < 4; i++) {
        int r = (edges[i] - 1) / 3, c = (edges[i] - 1) % 3;
        if (board[r][c] != 'X' && board[r][c] != 'O') return edges[i];
    }

    return 0;
}

static void playPvP() {
    char currentPlayer = 'X';
    bool gameOver = false;

    std::cout << "  Player 1: X | Player 2: O\n";

    while (!gameOver) {
        displayBoard();
        int move;
        std::cout << "  Player " << currentPlayer << ", enter position (1-9): ";

        if (!(std::cin >> move)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input. Try again.\n";
            continue;
        }

        if (!makeMove(move, currentPlayer)) {
            std::cout << "  Invalid move. Try again.\n";
            continue;
        }

        if (checkWin(currentPlayer)) {
            displayBoard();
            std::cout << "  Player " << currentPlayer << " wins!\n\n";
            gameOver = true;
        } else if (checkDraw()) {
            displayBoard();
            std::cout << "  It's a draw!\n\n";
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
}

static void playPvAI() {
    char player = 'X';
    char ai = 'O';
    bool gameOver = false;

    std::cout << "  You: X | Computer: O\n";

    while (!gameOver) {
        displayBoard();
        int move;
        std::cout << "  Your turn (1-9): ";

        if (!(std::cin >> move)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input. Try again.\n";
            continue;
        }

        if (!makeMove(move, player)) {
            std::cout << "  Invalid move. Try again.\n";
            continue;
        }

        if (checkWin(player)) {
            displayBoard();
            std::cout << "  You win!\n\n";
            gameOver = true;
            continue;
        } else if (checkDraw()) {
            displayBoard();
            std::cout << "  It's a draw!\n\n";
            gameOver = true;
            continue;
        }

        std::cout << "  Computer is thinking...\n";
        int aiMove = getAIMove();
        if (aiMove > 0) {
            makeMove(aiMove, ai);
            std::cout << "  Computer chose position " << aiMove << "\n";
        }

        if (checkWin(ai)) {
            displayBoard();
            std::cout << "  Computer wins!\n\n";
            gameOver = true;
        } else if (checkDraw()) {
            displayBoard();
            std::cout << "  It's a draw!\n\n";
            gameOver = true;
        }
    }
}

void playTicTacToe() {
    bool playAgain = true;
    while (playAgain) {
        initBoard();
        int mode;
        std::cout << "  ===== TIC TAC TOE =====\n\n";
        std::cout << "  1. Player vs Player\n";
        std::cout << "  2. Player vs Computer\n";
        std::cout << "  3. Back to Main Menu\n";
        std::cout << "\n  Choose mode: ";

        if (!(std::cin >> mode)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input.\n";
            continue;
        }

        switch (mode) {
            case 1:
                playPvP();
                break;
            case 2:
                playPvAI();
                break;
            case 3:
                return;
            default:
                std::cout << "  Invalid choice.\n";
                continue;
        }

        char replay;
        std::cout << "  Play again? (y/n): ";
        std::cin >> replay;
        playAgain = (replay == 'y' || replay == 'Y');
        std::cout << "\n";
    }
}
