#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class TicTacToe {
private:
    char board[3][3];
    int currentPlayer;
    int totalMoves;
    bool gameOver;
    bool gameWon;
    Vector2f winStart, winEnd;
    Font font;
    Text text;
    RectangleShape restartButton, exitButton;
    Text restartText, exitText;
 
public:
    TicTacToe() {
        currentPlayer = 1;
        totalMoves = 0;
        gameOver = false;
        gameWon = false;
        setupBoard();

        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Error loading font!" << endl;
            exit(1);
        }

        text.setFont(font);
        text.setCharacterSize(50);
        text.setFillColor(Color::White);

        // Restart Button
        restartButton.setSize(Vector2f(150, 50));
        restartButton.setFillColor(Color(100, 200, 100));
        restartButton.setPosition(75, 350);
        restartText.setFont(font);
        restartText.setCharacterSize(24);
        restartText.setFillColor(Color::White);
        restartText.setString("Restart");
        restartText.setPosition(110, 360);

        // Exit Button
        exitButton.setSize(Vector2f(150, 50));
        exitButton.setFillColor(Color(200, 100, 100));
        exitButton.setPosition(225, 350);
        exitText.setFont(font);
        exitText.setCharacterSize(24);
        exitText.setFillColor(Color::White);
        exitText.setString("Exit");
        exitText.setPosition(270, 360);
    }

    void setupBoard() {
        char num = '1';
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                board[row][col] = num++;
            }
        }
    }

    void resetBoard() {
        setupBoard();
        totalMoves = 0;
        gameOver = false;
        gameWon = false;
    }

    void drawBoard(RenderWindow &window) {
        window.clear(Color(173, 216, 230));

        // Draw Grid
        for (int i = 1; i < 3; i++) {
            RectangleShape glow(Vector2f(300, 10));
            glow.setFillColor(Color(255, 0, 0, 120));
            glow.setPosition(50, 50 + i * 100);
            window.draw(glow);
            glow.setSize(Vector2f(10, 300));
            glow.setPosition(50 + i * 100, 50);
            window.draw(glow);
        }

        // Draw X's and O's
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == 'X' || board[row][col] == 'O') {
                    text.setString(board[row][col]);
                    text.setPosition(70 + col * 100, 50 + row * 100);
                    window.draw(text);
                }
            }
        }

        // Draw winning line if game is won
        if (gameWon) {
            RectangleShape winLine(Vector2f(300, 5));
            winLine.setFillColor(Color(200, 100, 255, 180));
            winLine.setPosition(winStart);
            if (winStart.x == winEnd.x) {
    winLine.setSize(Vector2f(5, 300)); // Vertical win line
} 
else if (winStart.y == winEnd.y) {
    winLine.setSize(Vector2f(300, 5)); // Horizontal win line
} 
   else {
    winLine.setSize(Vector2f(360, 5)); 

    if (winStart.x == 60 && winEnd.x == 340) { 
        winLine.setRotation(45);  // Top-left to bottom-right
    } 
    else {  
        winLine.setRotation(-45); // Top-right to bottom-left
        winLine.setPosition(winStart.x - 5, winStart.y + 5); // Adjust position
    }
}


            window.draw(winLine);
        }

        if (gameOver) {
            window.draw(restartButton);
            window.draw(exitButton);
            window.draw(restartText);
            window.draw(exitText);
        }

        window.display();
    }

    bool checkWin() {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && isalpha(board[i][0])) {
                winStart = Vector2f(60, 100 + i * 100);
                winEnd = Vector2f(340, 100 + i * 100);
                gameWon = true;
                return true;
            }
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && isalpha(board[0][i])) {
                winStart = Vector2f(100 + i * 100, 60);
                winEnd = Vector2f(100 + i * 100, 340);
                gameWon = true;
                return true;
            }
        }
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && isalpha(board[0][0])) {
            winStart = Vector2f(60, 60);
            winEnd = Vector2f(340, 340);
            gameWon = true;
            return true;
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && isalpha(board[0][2])) {
            winStart = Vector2f(340, 60);
            winEnd = Vector2f(60, 340);
            gameWon = true;
            return true;
        }
        return false;
    }

    void startGame() {
        RenderWindow window(VideoMode(400, 450), "Tic-Tac-Toe");

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                if (gameOver && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (restartButton.getGlobalBounds().contains(mousePos)) {
                        resetBoard();
                    } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
                if (!gameOver && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int col = (event.mouseButton.x - 50) / 100;
                    int row = (event.mouseButton.y - 50) / 100;
                    if (col >= 0 && col < 3 && row >= 0 && row < 3 && isdigit(board[row][col])) {
                        board[row][col] = (currentPlayer == 1) ? 'X' : 'O';
                        totalMoves++;
                        if (checkWin() || totalMoves == 9) gameOver = true;
                        currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    }
                }
            }
            drawBoard(window);
        }
    }
};

int main() {
    TicTacToe game;
    game.startGame();
    return 0;
}

