#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h> // For terminal handling
#include <unistd.h>  // For usleep

using namespace std;

const int width = 80;
const int height = 20;

int x, y;
int fruitCordX, fruitCordY;
int playerScore;
int snakeTailX[100], snakeTailY[100];
int snakeTailLen;

enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakesDirection sDir;

bool isGameOver;

void GameInit() {
    isGameOver = false;
    sDir = STOP;
    x = width / 2;
    y = height / 2;
    fruitCordX = rand() % width;
    fruitCordY = rand() % height;
    playerScore = 0;
    snakeTailLen = 0;
}

void GameRender(string playerName) {
    clear(); // Clear the screen
    for (int i = 0; i < width + 2; i++)
        printw("-");
    printw("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {
            if (j == 0 || j == width)
                printw("|");
            else if (i == y && j == x)
                printw("0");
            else if (i == fruitCordY && j == fruitCordX)
                printw("#");
            else {
                bool printTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j && snakeTailY[k] == i) {
                        printw("o");
                        printTail = true;
                    }
                }
                if (!printTail)
                    printw(" ");
            }
        }
        printw("\n");
    }

    for (int i = 0; i < width + 2; i++)
        printw("-");
    printw("\n");

    printw("%s's Score: %d\n", playerName.c_str(), playerScore);
    refresh();
}

void UpdateGame() {
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;
    snakeTailX[0] = x;
    snakeTailY[0] = y;

    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (sDir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    if (x >= width || x < 0 || y >= height || y < 0)
        isGameOver = true;

    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y)
            isGameOver = true;
    }

    if (x == fruitCordX && y == fruitCordY) {
        playerScore += 10;
        fruitCordX = rand() % width;
        fruitCordY = rand() % height;
        snakeTailLen++;
    }
}

int SetDifficulty() {
    int dfc;
    char choice;

    cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard\n";
    cout << "NOTE: If no valid choice is made, difficulty will be set to medium.\n";
    cout << "Choose difficulty level: ";

    cin >> choice;

    switch (choice) {
    case '1':
        dfc = 150;
        break;
    case '2':
        dfc = 100;
        break;
    case '3':
        dfc = 50;
        break;
    default:
        dfc = 100;
    }
    return dfc;
}

void UserInput() {
    int ch = getch(); // Non-blocking input
    switch (ch) {
    case 'a':
        sDir = LEFT;
        break;
    case 'd':
        sDir = RIGHT;
        break;
    case 'w':
        sDir = UP;
        break;
    case 's':
        sDir = DOWN;
        break;
    case 'x':
        isGameOver = true;
        break;
    }
}

int main() {
    srand(time(0));
    string playerName;
    cout << "Enter your name: ";
    cin >> playerName;

    int dfc = SetDifficulty();

    initscr();            // Initialize the ncurses screen
    cbreak();             // Disable line buffering
    noecho();             // Disable echoing
    curs_set(0);          // Hide the cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking
    keypad(stdscr, TRUE); // Enable special keys

    GameInit();
    while (!isGameOver) {
        GameRender(playerName);
        UserInput();
        UpdateGame();
        usleep(dfc*1000); // Sleep in microseconds
    }

    endwin(); // Restore the terminal to normal mode
    cout << "Game Over! Final Score: " << playerScore << endl;
    return 0;
}

