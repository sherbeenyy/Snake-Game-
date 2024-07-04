
#include "raylib.h"
#include "iostream"
#include "deque"
#include "raymath.h"




using namespace std;

const int screenWidth = 600;
const int screenHeight = 600;

int cellSize = 30;
int cellCount = 20;


Color purple = { 204, 204, 255 , 255 };
Color darkBlue = { 70, 130, 180 , 255 };


//delaying the game
double lastTimeUpdated = 0;
bool timePassed(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastTimeUpdated >= interval) {
        lastTimeUpdated = currentTime;
        return true;
    }
    return false;
}

// searching for taget in given deque
bool elementInDeque(Vector2 target, deque<Vector2>searchIn) {
    for (int i = 0; i < searchIn.size(); i++) {
        if (Vector2Equals(searchIn[i], target)) {
            return true;
        }

    }
    return false;
}


// classes
class Snake {
public:
    deque<Vector2> body = { Vector2{6,8}, Vector2{5,8},Vector2{4,8} };
    Vector2 direction{ 1,0 };
    bool addPixel = false;

    void draw() {
        for (int i = 0; i < body.size(); i++) {

            int x = body[i].x;
            int y = body[i].y;
            DrawRectangle(x * cellSize, (float)y * cellSize, (float)cellSize, (float)cellSize, darkBlue);
          
        }
    }

    void update() {
        if (addPixel == true) {
            body.push_front(Vector2Add(body[0], direction));
            addPixel = false;
        }
        else {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
       
    }
    void reset() {
        body = { Vector2{6,9}, Vector2{5,9},Vector2{4,9} };
         direction = { 1,0 };
    }
};
class Apple {
public:
    Vector2 currentPosition;

    void draw() {
        Rectangle rec = Rectangle{ (float)currentPosition.x * cellSize,(float)currentPosition.y * cellSize,(float)cellSize, (float)cellSize };
        DrawRectangleRounded(rec, 0.6, 6, RED);
    }

    Apple(deque<Vector2> snakeBody) {
        currentPosition = randomPos(snakeBody);
    }
  
    Vector2 randomPos(deque<Vector2> snakeBody) {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        Vector2 newPostion = { x,y };
        while (elementInDeque(newPostion, snakeBody)) {
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            Vector2 newPostion = { x,y };
        }
        return newPostion;
    }
};
class Play {
public:
    Apple apple = Apple(snake.body);
    Snake snake = Snake();
    bool pause = false;
    int score = 0;
    int highScore = 0;

    

    void draw() {
        apple.draw();
        snake.draw();
    }

    void update() {
        if (!pause) {
            snake.update();
            eatingApple();
            collisionWithWall();
            collisionWithbody();
            highscore(score);
 
            
           
        }
        else {
            DrawText("Game Over !\n\nPress N to start a new game", 150, screenHeight/2, 22, RED);
        }
       
    }

    void eatingApple() {
        if (Vector2Equals(snake.body[0], apple.currentPosition)) {
           apple.currentPosition= apple.randomPos(snake.body);
           snake.addPixel = true;
           score++;
        }
    }

    void collisionWithWall() {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1) {
            endGame();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
            endGame();
        }
    }

    void collisionWithbody() {
        deque<Vector2> bodyWithoutHead = snake.body;
        bodyWithoutHead.pop_front();
        if (elementInDeque(snake.body[0], bodyWithoutHead)) {
            endGame();
        }
    }

    void endGame() {
        snake.reset();
        apple.currentPosition = apple.randomPos(snake.body);
        score = 0;
        pause = true;
    }
    void highscore(int score) {
        if (score > highScore) {
            highScore = score;
        }
    }
    


    

};


int main(void)
{

    InitWindow(screenWidth, screenHeight, "Snakey Snakey");
    SetTargetFPS(60);

  
    Play play;
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
        ClearBackground(purple);
        if (play.pause == false) {
            if (timePassed(0.1)) {
                play.update();
               
            }
        }
        else {
            play.update();
        }
       

        if (IsKeyPressed(KEY_W) && play.snake.direction.y != 1) {
            play.snake.direction = { 0,-1 };
        }
        if (IsKeyPressed(KEY_S) && play.snake.direction.y != -1) {
            play.snake.direction = { 0, 1 };
        }
        if (IsKeyPressed(KEY_D) && play.snake.direction.x != -1) {
            play.snake.direction = { 1, 0 };
        }
        if (IsKeyPressed(KEY_A) && play.snake.direction.x != 1) {
            play.snake.direction = { -1, 0 };
        }
        if (IsKeyPressed(KEY_N)) {
            play.pause = false;
        }
        DrawText(TextFormat(" Score : %i", play.score), 20, 500, 20, BLACK);
        DrawText(TextFormat(" High score : %i", play.highScore), 20, 530, 20, BLACK);
        play.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

