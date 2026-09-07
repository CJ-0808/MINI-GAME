#include <iostream>
#include <limits>
#include "TicTacToe.h"
#include "SnakeGame.h"

void displayBanner() {
    std::cout << "\n";
    std::cout << "  ===============================\n";
    std::cout << "     MINI GAMES COLLECTION\n";
    std::cout << "  ===============================\n";
    std::cout << "\n";
}

int main() {
    int choice;
    bool running = true;

    while (running) {
        displayBanner();
        std::cout << "  1. Tic Tac Toe\n";
        std::cout << "  2. Snake Game\n";
        std::cout << "  3. Exit\n";
        std::cout << "\n  Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n  Invalid input. Please enter a number.\n";
            continue;
        }

        std::cout << "\n";

        switch (choice) {
            case 1:
                playTicTacToe();
                break;
            case 2:
                playSnakeGame();
                break;
            case 3:
                running = false;
                std::cout << "  Thanks for playing! Goodbye.\n\n";
                break;
            default:
                std::cout << "  Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
