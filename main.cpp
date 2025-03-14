#include <cctype>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <ncurses.h>
#include <chrono>
#include <thread>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 15;

enum Direction {UP, DOWN, LEFT, RIGHT};

struct Point {
    int x, y;
};

class SnakeGame {
private:
    Point food;
    Point head;
    deque<Point> snake;
    Direction dir;
    bool gameOver;

public:
    SnakeGame() {
        gameOver = false;
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        dir = UP;
    }

    void input() {
        int ch = tolower(getch());

        if (ch == 'a' && dir != RIGHT) dir = LEFT;
        else if (ch == 'd' && dir != LEFT) dir = RIGHT;
        else if (ch == 'w' && dir != DOWN) dir = UP;
        else if (ch == 's' && dir != UP) dir = DOWN;
        else if (ch == 27) gameOver = true;
        
    }

    void move() {
        head = snake.front();

        switch (dir) {
            case LEFT:
                head.x -= 1;
                break;
            case RIGHT:
                head.x += 1;
                break;
            case UP:
                head.y -= 1;
                break;
            case DOWN:
                head.y += 1;
                break;
        }
        snake.push_front(head);
        snake.pop_back();
    }

    void draw() {
        clear();

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                    printw("#");
                } else if (snake.front().x == x && snake.front().y == y) {
                    printw("O");
                } else {
                    printw(" ");
                }
            }
            printw("\n");
            refresh();
        }
    }

    void run() {
        initscr();
        scrollok(stdscr, true);
        noecho();
        timeout(0);

        while (!gameOver) {
            draw();
            this_thread::sleep_for(chrono::milliseconds(400));
            input();
            move();
        }
        endwin();
    }
};

int main() {
    srand(time(0));
    SnakeGame game;
    game.run();
    return 0;
}
