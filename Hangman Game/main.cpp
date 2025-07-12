#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
using namespace std;

class Hangman {
private:
    int guessCount;
    bool gameOver;
    string guessedLetters;
    string wordToGuess;

public:
    Hangman() : guessCount(0), gameOver(false), guessedLetters(""), wordToGuess("") {}

    void PrintMessage(string message, bool printTop = true, bool printBottom = true) {
        if (printTop) cout << "+------------------------------------+\n|";
        else cout << "|";

        bool front = true;
        while (message.length() < 36) {
            if (front) message = " " + message;
            else message += " ";
            front = !front;
        }
        cout << message;

        if (printBottom) cout << "|\n+------------------------------------+\n";
        else cout << "|\n";
    }

    void DrawHangman() {
        static const vector<string> stages = {
            "", "!", "!", "O", "|", "\\|", "\\|/", "|", "/", "/\\"
        };
        for (int i = 0; i < 10; ++i) {
            if (guessCount > i) PrintMessage(stages[i], false, false);
            else PrintMessage("", false, false);
        }
    }

    void RandomWord(string path = "words.txt") {
        srand(static_cast<unsigned int>(time(0)));
        vector<string> words;
        ifstream file(path);

        if (!file.is_open()) {
            PrintMessage("ERROR: words.txt not found!");
            exit(1);
        }

        string word;
        while (getline(file, word)) {
            if (!word.empty()) words.push_back(word);
        }
        file.close();

        if (words.empty()) {
            PrintMessage("ERROR: words.txt is empty!");
            exit(1);
        }

        wordToGuess = words[rand() % words.size()];
    }

    void PrintLetters(char from, char to) {
        string line;
        for (char c = from; c <= to; c++) {
            if (guessedLetters.find(c) == string::npos)
                line += c;
            else
                line += ' ';
            line += ' ';
        }
        PrintMessage(line, false, false);
    }

    void RemainingLetters() {
        PrintMessage("REMAINING LETTERS");
        PrintLetters('a', 'm');
        PrintLetters('n', 'z');
    }

    void PrintWord() {
        PrintMessage("GUESS THE WORD");
        string display;
        gameOver = true;
        for (char c : wordToGuess) {
            if (guessedLetters.find(c) == string::npos) {
                display += "_ ";
                gameOver = false;
            } else {
                display += c;
                display += " ";
            }
        }
        PrintMessage(display, false);
    }

    void CountGuess(char guess) {
        if (wordToGuess.find(guess) == string::npos)
            guessCount++;
    }

    void Setup() {
        RandomWord();
        do {
            system("cls");
            PrintMessage("MADE BY SIDHARTH");
            DrawHangman();
            RemainingLetters();
            PrintWord();

            string input;
            cout << "> ";
            cin >> input;
            if (input.length() != 1 || !isalpha(input[0])) {
                PrintMessage("Enter one alphabet character.");
                _getch();
                continue;
            }

            char ch = tolower(input[0]);
            if (guessedLetters.find(ch) == string::npos) {
                guessedLetters += ch;
                CountGuess(ch);
            }

        } while (!gameOver && guessCount < 10);

        system("cls");
        if (gameOver)
            PrintMessage("YOU WIN!");
        else
            PrintMessage("YOU LOSE!");

        PrintMessage("The word was: " + wordToGuess);
        PrintMessage("Game Over");
    }
};

int main() {
    Hangman game;
    game.Setup();
    return 0;
} 
