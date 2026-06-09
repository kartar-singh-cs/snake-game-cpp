#include <iostream>
#include <raylib.h>
using namespace std;

int cellSize = 25; // Because we will move snake in to grid of 25 x 30 grid
int cellCount = 30;

int score = 0;

int margin = 75;

Color Green = Color{173, 204, 95, 255};
Color Dark_Green = Color{43, 51, 24, 255};

struct Node
{

    int x, y;
    Node *next;
};

class Snake
{
public:
    int dirX = 1;
    int dirY = 0;

    Node *head;
    Node *tail;

    Snake()
    {
        head = new Node{margin + 7 * cellSize, margin + 7 * cellSize, NULL};
        Node *body = new Node{margin + 6 * cellSize, margin + 7 * cellSize, NULL};
        tail = new Node{margin + 5 * cellSize, margin + 7 * cellSize, NULL};
        head->next = body;
        body->next = tail;
        tail->next = NULL;
    }

public:
    void Draw()
    {
        Node *temp = head;
        while (temp != NULL)
        {
            DrawRectangleRounded(Rectangle{(float)temp->x, (float)temp->y, (float)cellSize, (float)cellSize}, 0.5, 0, Dark_Green);
            temp = temp->next;
        }
    }

    // Adding head that will help to move snake forward with the help of dirX and dirY
    // if *dirX = 1,dirY = 0 then it will multiply with cellSize and addd in x of head that will create new head just after the previous head
    // by adding exact 1 cell size and it will move forward
    void AddHead()
    {
        int newX = head->x + dirX * cellSize;
        int newY = head->y + dirY * cellSize;

        if (newX > (cellCount * cellSize) + cellSize * 2) // Because of two cells we have to multiply it with 2 skip 2 cells more
        {
            newX = margin;
        }

        if (newX < margin)
        {
            newX = (cellCount * cellSize) + cellSize * 2;
        }

        if (newY > (cellCount * cellSize) + cellSize * 2)
        {
            newY = margin;
        }

        if (newY < margin)
        {
            newY = (cellCount * cellSize) + cellSize * 2;
        }

        Node *new_head = new Node{newX, newY, NULL};

        new_head->next = head;
        head = new_head;
    }

    // Removing Tail as normally we remove Tail from normal Linked List just as it is
    void removeTail()
    {
        if (head == NULL || head->next == NULL)
        {
            return;
        }

        Node *temp = head;
        while (temp->next->next != NULL)
        {
            temp = temp->next;
        }

        Node *helper = temp->next;
        temp->next = NULL;
        tail = temp;
        delete helper;
    }
};

class Food
{
public:
    Vector2 position;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{(float)position.x, (float)position.y, (float)cellSize, (float)cellSize}, 1, 0, RED);
    }
    Vector2 GenerateRandomPos(Node *head)
    {

        Vector2 pos;
        int row = GetRandomValue(0, cellCount - 1); // Because our gram has 29 rows
        int col = GetRandomValue(0, cellCount - 1);

        pos.x = margin + cellSize * col; // in Raylib Coordinates X is horizontal colums wise just Inverse of Matrices
        pos.y = margin + cellSize * row;

        Node *temp = head;

        while (temp != NULL)
        {
            if (temp->x == pos.x && temp->y == pos.y)
            {
                return Vector2{-1, -1};
            }

            temp = temp->next;
        }

        return pos;
    }
};

int main()
{

    InitWindow(margin * 2 + cellCount * cellSize, margin * 2 + cellCount * cellSize, "Snake By Kartar Singh");
    SetTargetFPS(5 + score / 2);

    Snake mysnake;
    Food food;

    food.position = food.GenerateRandomPos(mysnake.head); // Providing first place for food by Self made random function

    while (WindowShouldClose() == false)
    {
        // Updating
        BeginDrawing(); // Start Drawing from here

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && mysnake.dirY != 1)
        {
            mysnake.dirX = 0;
            mysnake.dirY = -1;
        }

        if (((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))) && mysnake.dirY != -1)
        {
            mysnake.dirX = 0;
            mysnake.dirY = 1;
        }

        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && mysnake.dirX != 1)
        {
            mysnake.dirX = -1;
            mysnake.dirY = 0;
        }

        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && mysnake.dirX != -1)
        {
            mysnake.dirX = 1;
            mysnake.dirY = 0;
        }

        Node *temp = mysnake.head->next;

        bool check = false;
        while (temp != NULL)
        {
            if (temp->x == mysnake.head->x && temp->y == mysnake.head->y)
            {
                check = true;
            }
            temp = temp->next;
        }

        if (check == false)
        {
            mysnake.AddHead();
            // Checking that if snake has eaten food or not
            bool ateFood = (mysnake.head->x == food.position.x && mysnake.head->y == food.position.y); // Both are in pixels because we have already multiply them with cellSize

            if (ateFood == false)
            {
                mysnake.removeTail();
            }

            else
            {

                Vector2 test = food.GenerateRandomPos(mysnake.head);

                while (test.x == -1)
                {
                    test = food.GenerateRandomPos(mysnake.head); // Checking if food is appear on snakes body so again generating coorect number and place fo its generating
                }

                food.position = test; // Keeping it equal to food Position
                score++;              // this means snake has eaten food and we should add score
            }

            ClearBackground(Green);

            // Drawing
            food.Draw();
            mysnake.Draw();

            DrawRectangleLinesEx(Rectangle{(float)margin, float(margin), (float)(cellCount * cellSize), (float)(cellCount * cellSize)}, 5, BLACK); // Drawing Black Square to give good frame

            DrawText(TextFormat("%i", score), margin, (cellSize * cellCount) + margin, 50, BLACK); // Drawing Score on right Bottom
            DrawText("RETRO SNAKE", margin, cellSize, 50, BLACK);                                  // DRAWING TEXT Just for showing RETRO SNAKE For Designing
        }

        if (check == true)
        {
            DrawText("GAME OVER", (margin + (cellCount * cellSize) / 2) - 300, margin + (cellCount * cellSize) / 2 - 40, 100, BLACK);
        }
        EndDrawing();
    }

    CloseWindow(); // Closing Window if user press esc or cross sign on window

    return 0;
}
