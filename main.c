#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

typedef float TimeStamp;

const int screenWidth = 1800;
const int screenHeight = 1000;
const int cellSize = 10;
const int columns = screenWidth / cellSize;;
const int lines = screenHeight / cellSize;

typedef struct StartPos
{
    const char** referense;
    int height;
    int width;
} StartPos;

const char* glider_referense[] =
{
    " # ",
    "  #",
    "###"
};
const StartPos glider = {glider_referense, 3, 3};

int Mod(int num, int mod)
{
    return ((num % mod) + mod) % mod;
}

typedef struct CellType
{
    bool isAlive;
    bool willLive;
} CellType;

typedef CellType** Field;

typedef enum DrawGridOptions
{
    DrawGrid_False = 0,
    DrawGrid_UnderLife,
    DrawGrid_AboveLife,
    DrawGrid_Max
} DrawGridOptions;

Field field;
bool isPaused = true;
DrawGridOptions drawGrid = DrawGrid_False;
float agesPerSecond = 5.0f;

void ToggleDrawGrid()
{
    drawGrid += 1;
    drawGrid %= DrawGrid_Max;
}


bool IsAlive(int x, int y)
{
    return field[Mod(x, columns)][Mod(y, lines)].isAlive;
}

bool WillLive(Field field, int x, int y)
{
    int alivesAround = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (IsAlive(x + i, y + j) && (i || j))
            {
                ++alivesAround;
            }
        }
    }

    if (alivesAround == 3)
    {
        return true;
    }
    else if (alivesAround == 2 && field[x][y].isAlive == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Field CreateField()
{
    Field field = calloc(columns, sizeof(CellType*));
    for (int i = 0; i < columns; ++i)
    {
        field[i] = calloc(lines, sizeof(CellType));
    }
    return field;
}

void LiveLife()
{
    for (int x = 0; x < columns; ++x)
    {
        for (int y = 0; y < lines; ++y)
        {
            field[x][y].willLive = WillLive(field, x, y); 
        }
    }

    for (int x = 0; x < columns; ++x)
    {
        for (int y = 0; y < lines; ++y)
        {
            field[x][y].isAlive = field[x][y].willLive;
        }
    }
}

void Clear(Field field)
{
    for (int x = 0; x < columns; ++x)
    {
        for (int y = 0; y < lines; ++y)
        {
            field[x][y] = (CellType){false, false};
        }
    }
}

void DrawLife()
{
    for (int x = 0; x < columns; ++x)
    {
        for (int y = 0; y < columns; ++y)
        {
            if (field[x][y].isAlive)
            {
                DrawRectangle(
                    cellSize * x,
                    cellSize * y,
                    cellSize,
                    cellSize,
                    RED
                );
            }
        }
    }
}


void SetStartPosition(Field field, StartPos pos, bool centered)
{
    int startX = 0;
    int startY = 0;
    if (centered)
    {
        startX = (columns - pos.width) / 2;
        startY = (lines - pos.height) / 2;
    }
    // in such representation actual X and Y axis are messed up
    // to set field intuitively just assume that field[x][y] is example[y][x]

    for (int x = 0; x < pos.height; ++x)
    {
        for (int y = 0; y < pos.width; ++y)
        {
            if (pos.referense[y][x] == '#')
            {
                field[x + startX][y + startY].isAlive = true;
            }
        }
    }
}

void DrawFieldGrid()
{
    for (int y = 0; y < lines; ++y)
    {
        DrawLine(0, y * cellSize, screenWidth, y * cellSize, BLACK);
    }
    for (int x = 0; x < columns; ++x)
    {
        DrawLine(x * cellSize, 0, x * cellSize, screenHeight, BLACK);
    }
}

void Draw()
{
    BeginDrawing();
    ClearBackground(BLUE);

    if (drawGrid == DrawGrid_UnderLife)
    {
        DrawFieldGrid();
    }

    DrawText("Press SPACE to Toggle Pause", 10, GetScreenHeight() - 210, 30, LIGHTGRAY);
    DrawText("Leftclick - Draw", 10, GetScreenHeight() - 180, 30, LIGHTGRAY);
    DrawText("Rightclick - Erase", 10, GetScreenHeight() - 150, 30, LIGHTGRAY);
    DrawText("Press N to Live 1 Age", 10, GetScreenHeight() - 120, 30, LIGHTGRAY);
    DrawText("Press G to Toggle Grid", 10, GetScreenHeight() - 90, 30, LIGHTGRAY);
    DrawText("Press C to Clear screen", 10, GetScreenHeight() - 60, 30, LIGHTGRAY);
    DrawText("Choose speed with 1 - 9", 10, GetScreenHeight() - 30, 30, LIGHTGRAY);

    DrawLife();

    if (drawGrid == DrawGrid_AboveLife)
    {
        DrawFieldGrid();
    }

    DrawFPS(10, 10);
    EndDrawing();
}

int RelativeCellX(float x)
{
    return (int)(x / cellSize);
}

int RelativeCellY(float y)
{
    return (int)(y / cellSize);
}

void HandleMouseActions(Field field)
{
    Vector2 tip = GetMousePosition();
    CellType* cell = &field[RelativeCellX(tip.x)][RelativeCellY(tip.y)];

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        cell->isAlive = true;
    }
    else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        cell->isAlive = false;
    }
}

void HandleSpeedControl()
{
    int key = GetKeyPressed();
    if (key >= 49 && key <= 57)
    {
        switch (key - 48)
        {
            case 1: agesPerSecond = 1.0f; break;
            case 2: agesPerSecond = 1.5f; break;
            case 3: agesPerSecond = 3.0f; break;
            case 4: agesPerSecond = 5.0f; break;
            case 5: agesPerSecond = 7.0f; break;
            case 6: agesPerSecond = 10.f; break;
            case 7: agesPerSecond = 20.f; break;
            case 8: agesPerSecond = 40.f; break;
            case 9: agesPerSecond = 60.f; break;
            default: break;
        }
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Bouncing ball");
    SetTargetFPS(120);

    field = CreateField();
    SetStartPosition(field, glider, true);
    
    TimeStamp now = GetTime();

    while (!WindowShouldClose())
    {
        Draw();

        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }

        if (isPaused && IsKeyPressed(KEY_N))
        {
            LiveLife();
        }

        if (IsKeyPressed(KEY_C))
        {
            Clear(field);
        }

        if (IsKeyPressed(KEY_G))
        {
            ToggleDrawGrid();
        }


        HandleSpeedControl();
        HandleMouseActions(field);

        if (GetTime() - now >= 1.0f / agesPerSecond && !isPaused)
        {
            LiveLife();
            now = GetTime();
        }
    }

    CloseWindow();
    return 0;
}
