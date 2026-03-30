#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int x, y, fruitX, fruitY, score, highScore;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;
HANDLE hConsole;

void LoadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void SaveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
}

void Draw() {
    system("cls");
    SetConsoleTextAttribute(hConsole, 15); // white for walls
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) {
                SetConsoleTextAttribute(hConsole, 15);
                printf("#");
            } else if (i == y && j == x) {
                SetConsoleTextAttribute(hConsole, 2); // green for head
                printf("O");
            } else if (i == fruitY && j == fruitX) {
                SetConsoleTextAttribute(hConsole, 4); // red for fruit
                printf("F");
            } else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetConsoleTextAttribute(hConsole, 6); // yellow for body
                        printf("o");
                        print = 1;
                    }
                }
                if (!print) {
                    SetConsoleTextAttribute(hConsole, 0); // black for space
                    printf(" ");
                }
            }

            if (j == WIDTH - 1) {
                SetConsoleTextAttribute(hConsole, 15);
                printf("#");
            }
        }
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 15);
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    printf("Score:%d High Score:%d\n", score, highScore);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = 1;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
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
    default:
        break;
    }
    if (x > WIDTH || x < 0 || y > HEIGHT || y < 0)
        gameOver = 1;
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    if (x == fruitX && y == fruitY) {
        score += 10;
        if (score > highScore) highScore = score;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));
    LoadHighScore();
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
    SaveHighScore();
    return 0;
}
