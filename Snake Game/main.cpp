#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

int gameType;
bool gameOver;

const int height = 25;
const int width = 60;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
direction dir;

void Setup() {
    cout << "1 | killer walls\n2 | transparent walls\n-->";
    cin >> gameType;

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;

    bool onSnake;
    do {
        fruitX = rand() % width;
        fruitY = rand() % height;
        onSnake = (fruitX == x && fruitY == y);
    } while (onSnake);

    score = 0;
    nTail = 0;
}

void Draw() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

    for (int i = 0; i < width + 2; i++) cout << "\xDC";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "\xDB";

            if (i == y && j == x) cout << "O";
            else if (i == fruitY && j == fruitX) cout << "$";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "0";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1) cout << "\xDB";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "\xDF";

    cout << "\nScore = " << score;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break;
        default: break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int tempX, tempY;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        tempX = tailX[i];
        tempY = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    if (gameType == 1) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            gameOver = true;
    } else if (gameType == 2) {
        if (x >= width) x = 0;
        else if (x < 0) x = width - 1;
        if (y >= height) y = 0;
        else if (y < 0) y = height - 1;
    }

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        bool onSnake;
        do {
            fruitX = rand() % width;
            fruitY = rand() % height;
            onSnake = (fruitX == x && fruitY == y);
            for (int i = 0; i < nTail && !onSnake; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY)
                    onSnake = true;
            }
        } while (onSnake);
        nTail++;
    }
}

int main() {
    while (true) {
        system("cls");
        Setup();

        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(80); // add delay for better visual experience
        }

        system("cls");
        cout << "____________GAME OVER____________\n";
        cout << "      Better Luck Next Time\n";
        cout << "SCORE: " << score;
        cout << "\nPress 'y' to play again, any other key to exit: ";

        char ch = _getch();
        if (ch == 'y' || ch == 'Y')
            continue;
        else
            break;
    }

    return 0;
}
