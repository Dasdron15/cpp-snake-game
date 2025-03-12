#include <iostream>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <termios.h>
#include <unistd.h>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

enum Direction {UP, DOWN, LEFT, RIGHT};

struct Point {
    int x, y;
};

class SnakeGame {
private:
    Point food;
    deque<Point> snake;
    Direction dir;
    bool gameOver;

public:
    SnakeGame() {
        gameOver = false;
        snake.push_back({WIDTH / 2, HEIGHT / 2});
    }

    char getKey() {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);  // Сохранение текущих настроек терминала
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);  // Отключаем буферизацию и эхо-вывод
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Восстановление настроек
        return ch;
    }

    void input() {
        if (getKey() == 'a') {
            snake.front().x -= 1;
        } else if (getKey() == 'd') {
            snake.front().x += 1;
        } else if (getKey() == 'w') {
            snake.front().y -= 1;
        } else if (getKey() == 's') {
            snake.front().y += 1;
        }
    }

    void draw() {
        system("clear");

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                    cout << "#";
                } else if (snake.front().x == x && snake.front().y == y) {
                    cout << "O";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
        }
        cout << "Game Over!" << endl;
    }
};

int main() {
    srand(time(0));
    SnakeGame game;
    game.run();
    return 0;
}
