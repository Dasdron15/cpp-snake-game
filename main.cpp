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
    int score;

public:
    SnakeGame() {
        score = 1;
        gameOver = false;
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        dir = UP;
        spawnFood();
    }

    void input() {
        int ch = tolower(getch());

        if (ch == 'a' && dir != RIGHT) dir = LEFT;
        else if (ch == 'd' && dir != LEFT) dir = RIGHT;
        else if (ch == 'w' && dir != DOWN) dir = UP;
        else if (ch == 's' && dir != UP) dir = DOWN;
        else if (ch == 27) endwin();
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

    void checkDeath() {
        if (snake.front().x == 0 || snake.front().x == WIDTH - 1 || snake.front().y == 0 || snake.front().y == HEIGHT - 1) {
            gameOver = true;
        }
    }

    void spawnFood() {
        if (snake.front().x == food.x && snake.front().y == food.y) {
            score += 1;
            food.x = 1 + (rand() % WIDTH - 2);
            food.y = 1 + (rand() % HEIGHT - 2);
        }
    }

    void draw() {
        clear();
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                    printw("#");
                } else if (snake.front().x == x && snake.front().y == y) {
                    printw("*");
                } else if (food.x == x && food.y == y) {
                    printw("O");
                } else {
                    printw(" ");
                }
            }
            printw("\n");
            refresh();
        }
        mvprintw(HEIGHT, 0, "Your score is: 6");
    }

    void run() {
        initscr();
        scrollok(stdscr, true);
        noecho();
        timeout(0);

        while (!gameOver) {
            checkDeath();
            spawnFood();
            draw();
            this_thread::sleep_for(chrono::milliseconds(300));
            input();
            move();
        }
        timeout(-1);
        mvprintw(HEIGHT + 1, 0, "Game over! Press any key to exit.");
        getch();
        endwin();
    }
};

int main() {
    srand(time(0));
    SnakeGame game;
    game.run();
    return 0;
}
