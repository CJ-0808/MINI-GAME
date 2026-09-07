#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include "SnakeGame.h"

struct Point {
    int x;
    int y;
};

static const int WI = 20;
static const int HE = 14;
static bool grid[WI][HE];

static termios orig_tty;

struct Direction {
    int dx;
    int dy;
};

static void clearScreen() {
    std::cout << "\033[2J\033[H";
}

static void hideCursor() {
    std::cout << "\033[?25l";
}

static void showCursor() {
    std::cout << "\033[?25h";
}

static void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_tty);
}

static void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_tty);
    termios raw = orig_tty;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit(disableRawMode);
}

static char readInput() {
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    if (c == '\x1b') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': return 'U';
                    case 'B': return 'D';
                    case 'C': return 'R';
                    case 'D': return 'L';
                }
            }
        }
        return 0;
    }
    return c;
}

static void resetGrid() {
    for (int i = 0; i < WI; i++)
        for (int j = 0; j < HE; j++)
            grid[i][j] = false;
}

static bool isWall(int x, int y) {
    return x < 0 || x >= WI || y < 0 || y >= HE;
}

static void placeFood() {
    std::vector<Point> free;
    for (int i = 0; i < WI; i++)
        for (int j = 0; j < HE; j++)
            if (!grid[i][j]) free.push_back({i, j});

    if (free.empty()) return;

    int idx = rand() % free.size();
    grid[free[idx].x][free[idx].y] = true;
}

static void render(const std::vector<Point>& snake, int score) {
    clearScreen();
    std::cout << "\033[36m" << "  Snake Game" << "\033[0m\n";
    std::cout << "  Score: " << score << "\n\n";

    for (int y = -1; y <= HE; y++) {
        std::cout << "  ";
        for (int x = -1; x <= WI; x++) {
            bool isWallCel = (x == -1 || x == WI || y == -1 || y == HE);
            if (isWallCel) {
                std::cout << "\033[90m#\033[0m";
            } else {
                bool isSnake = false;
                for (size_t i = 0; i < snake.size(); i++) {
                    if (snake[i].x == x && snake[i].y == y) {
                        isSnake = true;
                        if (i == 0) std::cout << "\033[32m@\033[0m";
                        else std::cout << "\033[32mo\033[0m";
                        break;
                    }
                }
                if (!isSnake) {
                    if (grid[x][y]) std::cout << "\033[31m*\033[0m";
                    else std::cout << " ";
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n  Use arrow keys to move. Press 'x' to quit.\n";
    std::cout << std::flush;
}

void playSnakeGame() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "\033[2J\033[H\033[36m" << "  ===== SNAKE GAME =====\033[0m\n\n";
    std::cout << "  Controls: Arrow keys to move, 'x' to quit.\n";
    std::cout << "  Eat the red (*) food to grow and score.\n";
    std::cout << "  Avoid walls and yourself!\n\n";
    std::cout << "  Press ENTER to start...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    enableRawMode();
    hideCursor();

    bool playAgain = true;
    while (playAgain) {
        resetGrid();
        std::vector<Point> snake;
        snake.push_back({WI / 2, HE / 2});
        snake.push_back({WI / 2 - 1, HE / 2});
        snake.push_back({WI / 2 - 2, HE / 2});
        for (size_t i = 0; i < snake.size(); i++)
            grid[snake[i].x][snake[i].y] = true;

        Direction dir = {1, 0};
        Direction lastDir = dir;
        int score = 0;
        bool gameOver = false;
        char lastKey = 0;

                placeFood();
        render(snake, score);

        while (!gameOver) {
            usleep(80000);

            char key = readInput();
            if (key == 'x' || key == 'X') break;

            if (key == 'U' && lastDir.dy != 1) { dir = {0, -1}; }
            else if (key == 'D' && lastDir.dy != -1) { dir = {0, 1}; }
            else if (key == 'L' && lastDir.dx != 1) { dir = {-1, 0}; }
            else if (key == 'R' && lastDir.dx != -1) { dir = {1, 0}; }

            Point newHead = {snake.front().x + dir.dx, snake.front().y + dir.dy};
            lastDir = dir;

            if (isWall(newHead.x, newHead.y)) {
                gameOver = true;
                break;
            }

            bool ate = grid[newHead.x][newHead.y] && !(newHead.x == snake.front().x && newHead.y == snake.front().y);
            bool collision = false;
            if (!ate) {
                for (size_t i = 0; i < snake.size(); i++) {
                    if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
                        collision = true;
                        break;
                    }
                }
            }

            if (collision) {
                gameOver = true;
                break;
            }

            snake.insert(snake.begin(), newHead);
            grid[newHead.x][newHead.y] = true;

            if (ate) {
                score += 10;
        placeFood();
            } else {
                Point tail = snake.back();
                snake.pop_back();
                grid[tail.x][tail.y] = false;
            }

            render(snake, score);
        }

        showCursor();
        disableRawMode();

        if (lastKey != 'x' && lastKey != 'X') {
            std::cout << "\033[2J\033[H";
            std::cout << "\033[33m" << "  ===== GAME OVER =====\033[0m\n";
            std::cout << "  Final Score: " << score << "\n\n";
        } else {
            std::cout << "\033[2J\033[H";
        }

        char ch;
        std::cout << "  Play again? (y/n): ";
        std::cin >> ch;
        playAgain = (ch == 'y' || ch == 'Y');
        std::cout << "\n";
    }
}
