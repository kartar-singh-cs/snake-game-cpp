#include <iostream>
#include <raylib.h>
using namespace std;

Color Green = Color{173, 204, 95, 255};
Color Dark_Green = Color{43, 51, 24, 255};
int score = 0;

int cellSize = 25;
int cellCount = 30;
int margin = 75;

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

    Node *head = new Node;
    Node *tail = new Node;

    Snake()
    {
        Node *n1 = new Node{7 * cellSize, 7 * cellSize, NULL};
        Node *n2 = new Node{6 * cellSize, 7 * cellSize, NULL};
        Node *n3 = new Node{5 * cellSize, 7 * cellSize, NULL};

        n1->next = n2;
        n2->next = n3;

        head = n1;
        tail = n3;
    }

    void Draw()
    {
        Node *temp = head;
        while (temp != NULL)
        {
            DrawRectangleRounded(
                Rectangle{(float)temp->x, (float)temp->y, (float)cellSize, (float)cellSize},
                0.5, 0, Dark_Green);

            temp = temp->next;
        }
    }

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

        delete temp->next;
        temp->next = NULL;
        tail = temp;
    }

    void AddHead()
    {
        int newX = head->x + dirX * cellSize;
        int newY = head->y + dirY * cellSize;

        // Left boundary → teleport to right
        if (newX < margin)
            newX = margin + (cellCount - 1) * cellSize;

        // Right boundary → teleport to left
        if (newX >= margin + cellCount * cellSize)
            newX = margin;

        // Top boundary → teleport to bottom
        if (newY < margin)
            newY = margin + (cellCount - 1) * cellSize;

        // Bottom boundary → teleport to top
        if (newY >= margin + cellCount * cellSize)
            newY = margin;

        Node *newHead = new Node{newX, newY};
        newHead->next = head;
        head = newHead;
    }
};

// Food Class
class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        Image image = LoadImage("C:/raylib_pacman/food.png"); // Loading Image

        ImageResize(&image, cellSize, cellSize);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        float scale = 1.0f;
        DrawTextureEx(texture,
                      Vector2{position.x * cellSize, position.y * cellSize},
                      0.0f, scale, WHITE);
    }

    Vector2 GenerateRandompos(Node *head)
    {
        Vector2 pos;

        pos.x = GetRandomValue(margin / cellSize, margin / cellSize + cellCount - 1);
        pos.y = GetRandomValue(margin / cellSize, margin / cellSize + cellCount - 1);

        Node *temp = head;

        while (temp != NULL)
        {
            // FIX: compare correctly (pixel vs grid)
            if (temp->x == pos.x * cellSize && temp->y == pos.y * cellSize)
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
    bool gameover = false;
    double lastTime = 0;
    double moveDelay = 0.3;

   

    InitWindow(2 * margin + cellCount * cellSize, 2 * margin + cellCount * cellSize, "Snake Game By Kartar Singh");
    SetTargetFPS(60);

    Snake mysnake;
    Food food;

    food.position = food.GenerateRandompos(mysnake.head);

    while ((WindowShouldClose() == false))
    {
        BeginDrawing();

        // Updating
        if (((IsKeyPressed(KEY_W)) || (IsKeyPressed(KEY_UP))) && mysnake.dirY != 1)
        {
            mysnake.dirY = -1;
            mysnake.dirX = 0;
        }
        if (((IsKeyPressed(KEY_D)) || (IsKeyPressed(KEY_RIGHT))) && mysnake.dirX != -1)
        {
            mysnake.dirX = 1;
            mysnake.dirY = 0;
        }
        if (((IsKeyPressed(KEY_S)) || (IsKeyPressed(KEY_DOWN))) && mysnake.dirY != -1)
        {
            mysnake.dirY = 1;
            mysnake.dirX = 0;
        }
        if (((IsKeyPressed(KEY_A)) || (IsKeyPressed(KEY_LEFT))) && mysnake.dirX != 1)
        {
            mysnake.dirX = -1;
            mysnake.dirY = 0;
        }

        double currentTime = GetTime();

        if (gameover != true && currentTime - lastTime >= moveDelay)
        {
            mysnake.AddHead(); // Move snake by adding a new head

            bool check = false;

            Node *temp = mysnake.head->next;

            while (check != true && temp != NULL)
            {
                if (mysnake.head->x == temp->x && mysnake.head->y == temp->y)
                {
                    check = true;
                }
                temp = temp->next;
            }

            if (check == true)
            {
                gameover = true;
            }

            // ----------------------------
            // Check if snake ate the food
            // ----------------------------
            bool ateFood = (mysnake.head->x == food.position.x * cellSize &&
                            mysnake.head->y == food.position.y * cellSize);

            if (ateFood == false)
            {
                mysnake.removeTail(); // Remove tail if no food eaten
            }
            else
            {
                Vector2 newfoodpos = food.GenerateRandompos(mysnake.head);

                while (newfoodpos.x == -1)
                {
                    newfoodpos = food.GenerateRandompos(mysnake.head);
                }

                food.position = newfoodpos;
                score++;
            }

            lastTime = currentTime; // Reset move timer
        }

        ClearBackground(Green);

        // Drawing
        food.Draw();
        mysnake.Draw();
        // adjust to match the screenshot

        DrawRectangleLinesEx(Rectangle{(float)margin, (float)margin, (float)(cellCount * cellSize), (float)(cellCount * cellSize)}, 4, BLACK);

        DrawText(TextFormat("%i", score), margin, margin + cellCount * cellSize, 60, BLACK);
        DrawText("RETRO SNAKE", margin, cellSize, 50, BLACK);
        if (gameover == true)
        {
            DrawText("GAME OVER", (margin + (cellCount * cellSize) / 2) - 300, margin + (cellCount * cellSize) / 2 - 40, 100, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}