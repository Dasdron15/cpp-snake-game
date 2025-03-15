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
        food.x = 1 + rand() % (WIDTH - 2);
        food.y = 1 + rand() % (HEIGHT - 2);
    }

    void input() {
        int ch = tolower(getch());

        if (ch == 'a' && dir != RIGHT) dir = LEFT;
        else if (ch == 'd' && dir != LEFT) dir = RIGHT;
        else if (ch == 'w' && dir != DOWN) dir = UP;
        else if (ch == 's' && dir != UP) dir = DOWN;
        else if (ch == 27) {
            gameOver = true;
            endwin();
            exit(0);
        }
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
        
        if (head.x == food.x && head.y == food.y) {
            spawnFood();
        } else {
            snake.pop_back();
        }
    }

    void checkDeath() {
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].x == snake.front().x && snake[i].y == snake.front().y) {
                gameOver = true;
                return;
            }
        }
        if (snake.front().x == 0 || snake.front().x == WIDTH - 1 || snake.front().y == 0 || snake.front().y == HEIGHT - 1) {
            gameOver = true;
        }
    }

    void spawnFood() {
        food.x = 1 + rand() % (WIDTH - 2);
        food.y = 1 + rand() % (HEIGHT - 2);
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
                    bool isBody = false;
                    for (const auto& p : snake) {
                        if (x == p.x && y == p.y) {
                            printw("*");
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody) {
                        printw(" ");
                    }
                }
            }
            printw("\n");
        }
        refresh();
    }

    void run() {
        initscr();
        scrollok(stdscr, true);
        noecho();
        timeout(0);
        srand(time(0));

        while (!gameOver) {
            checkDeath();
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
    SnakeGame game;
    game.run();
    return 0;
}
