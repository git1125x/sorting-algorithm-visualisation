#include "raylib.h"
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <atomic>

// only for testing:
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define TIME_DELTA 0.01f
#define NUMBER_OF_BARS 20

#define X1_BORDER 20
#define Y1_BORDER 20
#define X2_BORDER SCREEN_WIDTH * 2 / 3 - 20
#define Y2_BORDER SCREEN_HEIGHT - 20

class Button
{
private:
    Vector2 pos_;
    Vector2 size_;
    bool buttonPressed;

public:
    Button(Vector2 pos, Vector2 size) : pos_(pos), size_(size), buttonPressed(false) { ; }
    void pressButton()
    {
        buttonPressed = !buttonPressed;
    }
    void draw()
    {
        if (buttonPressed)
        {
            DrawRectangleV(pos_, size_, DARKGRAY);
        }
        else
        {
            DrawRectangleV(pos_, size_, BLACK);
        }
    }
};

void drawBorder()
{
    Color col = BLACK;
    DrawLine(X1_BORDER, Y1_BORDER, X2_BORDER, Y1_BORDER, col);
    DrawLine(X2_BORDER, Y1_BORDER, X2_BORDER, Y2_BORDER, col);
    DrawLine(X2_BORDER, Y2_BORDER, X1_BORDER, Y2_BORDER, col);
    DrawLine(X1_BORDER, Y2_BORDER, X1_BORDER, Y1_BORDER, col);
}

bool numInNums(int num, std::vector<std::pair<int, Color>> numbers)
{
    if (numbers.size() == 0)
    {
        return false;
    }
    for (int i = 0; i < numbers.size(); ++i)
    {
        if (num == numbers[i].first)
        {
            return true;
        }
    }
    return false;
}

void drawBars(std::vector<std::pair<int, Color>> numbers)
{
    int x1 = X1_BORDER;
    int y1 = Y1_BORDER;
    int x2 = X2_BORDER;
    int y2 = Y2_BORDER;
    float offset = 5;
    float width = (float)x2 - (float)x1;
    float height = (float)y2 - (float)y1;
    float size = numbers.size();
    float offset_size = offset * (size + 1);
    float bar_width = (float)(width - offset_size) / (float)size;
    float min_bar_hight = (height - 2 * offset) / size;
    for (int i = 0; i < numbers.size(); ++i)
    {
        float bar_height = numbers[i].first * min_bar_hight;
        Vector2 position;
        Vector2 size;
        size.x = bar_width;
        size.y = bar_height;
        position.x = (float)x1 + offset * (i + 1) + i * bar_width;
        position.y = (float)y2 - bar_height;
        Color color = numbers[i].second;
        DrawRectangleV(position, size, color);
    }
}

void randomizeNumbers(std::vector<std::pair<int, Color>> &numbers)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(numbers.begin(), numbers.end(), gen);
}

std::vector<std::pair<int, Color>> generateNumbers(int size)
{
    std::vector<std::pair<int, Color>> result;
    for (int i = 1; i <= size; ++i)
    {
        std::pair<int, Color> tmp = std::make_pair(i, BLACK);
        result.emplace_back(tmp);
    }
    return result;
}
void swapPlaces(std::vector<std::pair<int, Color>> &numbers, int i, int j)
{
    if (i < 0 || j < 0 || i >= numbers.size() || j >= numbers.size())
    {
        std::cout << "Cant swapt, index out of range." << std::endl;
    }
    else
    {
        numbers[i].second = RED;
        numbers[i+1].second = DARKGRAY;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::swap(numbers[i], numbers[j]);
        numbers[i].second = DARKGRAY;
        numbers[i+1].second = RED;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        numbers[i].second = BLACK;
        numbers[i+1].second = BLACK;
    }
}

void sort(std::vector<std::pair<int, Color>> &numbers)
{
    int counter = 1;
    while (counter != 0)
    {
        counter = 0;
        for (int i = 0; i < numbers.size() - 1; ++i)
        {
            if (numbers[i].first > numbers[i + 1].first)
            {
                counter++;
                swapPlaces(numbers, i, i + 1);
            }
        }
    }
}

void visualize(std::vector<std::pair<int, Color>> &numbers, std::atomic_bool &running, std::atomic_bool &start_sorting, std::atomic_bool &sorted)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sorting Algorithms");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KeyboardKey::KEY_UP))
        {
            std::cout << "Key up pressed" << std::endl;
            numbers = generateNumbers(numbers.size() + 1);
        }
        if (IsKeyPressed(KeyboardKey::KEY_DOWN))
        {
            numbers = generateNumbers(numbers.size() - 1);
        }
        if (IsKeyPressed(KeyboardKey::KEY_LEFT))
        {
            randomizeNumbers(numbers);
            sorted = false;
        }
        if (IsKeyPressed(KeyboardKey::KEY_RIGHT) && !sorted)
        {
            std::cout << "sort the numbers" << std::endl;
            start_sorting = true;
            sorted = false;
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        // BEGIN: Draw Objects
        drawBorder();
        drawBars(numbers);
        // button.draw();
        //  END: Draw Objects
        EndDrawing();
        WaitTime(TIME_DELTA);
    }
    running = false;
    CloseWindow();
}

void sorting(std::vector<std::pair<int, Color>> &numbers, std::atomic_bool &running, std::atomic_bool &start_sorting, std::atomic_bool &sorted)
{
    while (running)
    {
        if (start_sorting)
        {
            std::cout << "Starting to sort" << std::endl;
            sort(numbers);
            start_sorting = false;
            sorted = true;
            std::cout << "Ended sorting" << std::endl;
        }
    }
}

int main(void)
{
    // BEGIN: initalize objects
    int numOfBars = 0;
    // GuiSliderBar((Rectangle){ 600, 40, 120, 20 }, "StartAngle", NULL, &numOfBars, 1, 100);
    //  define boarder variables
    const int x1 = 20;
    const int y1 = 20;
    const int x2 = SCREEN_WIDTH * 2 / 3 - 20;
    const int y2 = SCREEN_HEIGHT - 20;
    int size = 10;
    std::vector<std::pair<int, Color>> numbers = generateNumbers(size);
    Vector2 button_pos{SCREEN_WIDTH - 200, 100};
    Vector2 button_size{130, 40};
    Button button(button_pos, button_size);

    std::atomic_bool start_sorting(false);
    std::atomic_bool sorted(true);
    std::atomic_bool running(true);

    std::thread visualizationThread(visualize, std::ref(numbers), std::ref(running), std::ref(start_sorting), std::ref(sorted));
    std::thread sortingThread(sorting, std::ref(numbers),std::ref(running), std::ref(start_sorting), std::ref(sorted));
    visualizationThread.join();
    sortingThread.join();

    return 0;
}
