#include "raylib.h"
#include <string>
#include <chrono>
#include <thread>

struct Button {
    Rectangle rect;
    const char* text;
    Color color;
    bool isHovered;
};

class CountdownTimer {
private:
    double totalTime;      // Total time in seconds
    double remainingTime;  // Remaining time in seconds
    bool isRunning;
    bool isPaused;
    
    Button startButton;
    Button hourButton;
    Button twentyMinButton;
    Button oneMinButton;
    Button pauseButton;
    Button stopButton;
    
public:
    CountdownTimer() {
        totalTime = 0.0;
        remainingTime = 0.0;
        isRunning = false;
        isPaused = false;
        
        // Initialize buttons (positions will be set in Update based on screen width)
        startButton = { {0, 0, 120, 50}, "Start", GREEN, false };
        hourButton = { {0, 0, 120, 50}, "1 Hour", BLUE, false };
        twentyMinButton = { {0, 0, 120, 50}, "20 Min", BLUE, false };
        oneMinButton = { {0, 0, 120, 50}, "1 Min", BLUE, false };
        pauseButton = { {0, 0, 120, 50}, "Pause", ORANGE, false };
        stopButton = { {0, 0, 120, 50}, "Stop", RED, false };
    }
    
    void Update() {
        Vector2 mousePoint = GetMousePosition();
        
        // Update button positions based on screen width
        int screenWidth = GetScreenWidth();
        int buttonY1 = 350;  // First row Y position
        int buttonY2 = 420;  // Second row Y position
        int spacing = 20;
        int totalWidth = 120 * 3 + spacing * 2;  // 3 buttons in first row
        int startX = screenWidth / 2 - totalWidth / 2;
        
        // Position buttons in first row (1 Hour, 20 Min, 1 Min)
        hourButton.rect.x = startX;
        hourButton.rect.y = buttonY1;
        twentyMinButton.rect.x = startX + 120 + spacing;
        twentyMinButton.rect.y = buttonY1;
        oneMinButton.rect.x = startX + 240 + spacing * 2;
        oneMinButton.rect.y = buttonY1;
        
        // Second row buttons (centered - Start, Pause, Stop)
        int secondRowTotalWidth = 120 * 3 + spacing * 2;
        int secondRowStartX = screenWidth / 2 - secondRowTotalWidth / 2;
        startButton.rect.x = secondRowStartX;
        startButton.rect.y = buttonY2;
        pauseButton.rect.x = secondRowStartX + 120 + spacing;
        pauseButton.rect.y = buttonY2;
        stopButton.rect.x = secondRowStartX + 240 + spacing * 2;
        stopButton.rect.y = buttonY2;
        
        // Update button hover states
        UpdateButtonHover(startButton, mousePoint);
        UpdateButtonHover(hourButton, mousePoint);
        UpdateButtonHover(twentyMinButton, mousePoint);
        UpdateButtonHover(oneMinButton, mousePoint);
        UpdateButtonHover(pauseButton, mousePoint);
        UpdateButtonHover(stopButton, mousePoint);
        
        // Handle button clicks
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePoint, startButton.rect)) {
                if (!isRunning && remainingTime > 0) {
                    isRunning = true;
                    isPaused = false;
                } else if (isPaused) {
                    isRunning = true;
                    isPaused = false;
                }
            }
            else if (CheckCollisionPointRec(mousePoint, hourButton.rect)) {
                totalTime = 3600.0;  // 1 hour = 3600 seconds
                remainingTime = totalTime;
                isRunning = false;
                isPaused = false;
            }
            else if (CheckCollisionPointRec(mousePoint, twentyMinButton.rect)) {
                totalTime = 1200.0;  // 20 minutes = 1200 seconds
                remainingTime = totalTime;
                isRunning = false;
                isPaused = false;
            }
            else if (CheckCollisionPointRec(mousePoint, oneMinButton.rect)) {
                totalTime = 60.0;  // 1 minute = 60 seconds
                remainingTime = totalTime;
                isRunning = false;
                isPaused = false;
            }
            else if (CheckCollisionPointRec(mousePoint, pauseButton.rect)) {
                if (isRunning && !isPaused) {
                    isRunning = false;
                    isPaused = true;
                }
            }
            else if (CheckCollisionPointRec(mousePoint, stopButton.rect)) {
                remainingTime = 0;
                totalTime = 0;
                isRunning = false;
                isPaused = false;
            }
        }
        
        // Update timer
        if (isRunning && !isPaused && remainingTime > 0) {
            float deltaTime = GetFrameTime();
            remainingTime -= deltaTime;
            
            if (remainingTime <= 0) {
                remainingTime = 0;
                isRunning = false;
                isPaused = false;
            }
        }
    }
    
    void Draw() {
        ClearBackground(RAYWHITE);
        
        // Draw title
        DrawText("COUNTDOWN TIMER", GetScreenWidth() / 2 - MeasureText("COUNTDOWN TIMER", 40) / 2, 50, 40, DARKGRAY);
        
        // Draw time display
        DrawTimeDisplay();
        
        // Draw buttons
        DrawButton(hourButton);
        DrawButton(twentyMinButton);
        DrawButton(oneMinButton);
        DrawButton(startButton);
        DrawButton(pauseButton);
        DrawButton(stopButton);
        
        // Draw status
        DrawStatus();
    }
    
private:
    void UpdateButtonHover(Button& button, Vector2 mousePoint) {
        button.isHovered = CheckCollisionPointRec(mousePoint, button.rect);
    }
    
    void DrawButton(Button& button) {
        Color buttonColor = button.color;
        if (button.isHovered) {
            // Make color brighter when hovered
            buttonColor = ColorBrightness(buttonColor, 0.3f);
        }
        
        DrawRectangleRec(button.rect, buttonColor);
        DrawRectangleLinesEx(button.rect, 2, DARKGRAY);
        
        int textWidth = MeasureText(button.text, 20);
        DrawText(button.text, 
                button.rect.x + button.rect.width / 2 - textWidth / 2,
                button.rect.y + button.rect.height / 2 - 10,
                20, WHITE);
    }
    
    void DrawTimeDisplay() {
        int hours = static_cast<int>(remainingTime / 3600);
        int minutes = static_cast<int>((remainingTime - hours * 3600) / 60);
        int seconds = static_cast<int>(remainingTime - hours * 3600 - minutes * 60);
        
        std::string timeText;
        if (hours > 0) {
            if (hours < 10) timeText += "0";
            timeText += std::to_string(hours) + ":";
            if (minutes < 10) timeText += "0";
            timeText += std::to_string(minutes) + ":";
            if (seconds < 10) timeText += "0";
            timeText += std::to_string(seconds);
        } else {
            if (minutes < 10) timeText += "0";
            timeText += std::to_string(minutes) + ":";
            if (seconds < 10) timeText += "0";
            timeText += std::to_string(seconds);
        }
        
        int fontSize = 80;
        int textWidth = MeasureText(timeText.c_str(), fontSize);
        
        DrawText(timeText.c_str(), 
                GetScreenWidth() / 2 - textWidth / 2, 
                180, 
                fontSize, 
                remainingTime < 10 ? RED : DARKGREEN);
        
        // Draw progress bar
        if (totalTime > 0) {
            float progress = remainingTime / totalTime;
            int barWidth = 400;
            int barHeight = 20;
            int barX = GetScreenWidth() / 2 - barWidth / 2;
            int barY = 300;
            
            DrawRectangle(barX, barY, barWidth, barHeight, LIGHTGRAY);
            DrawRectangle(barX, barY, static_cast<int>(barWidth * progress), barHeight, 
                         remainingTime < 10 ? RED : GREEN);
            DrawRectangleLines(barX, barY, barWidth, barHeight, DARKGRAY);
        }
    }
    
    void DrawStatus() {
        const char* statusText;
        Color statusColor;
        
        if (remainingTime == 0 && totalTime > 0) {
            statusText = "TIME'S UP!";
            statusColor = RED;
        } else if (isPaused) {
            statusText = "PAUSED";
            statusColor = ORANGE;
        } else if (isRunning) {
            statusText = "RUNNING";
            statusColor = GREEN;
        } else {
            statusText = "STOPPED";
            statusColor = GRAY;
        }
        
        int textWidth = MeasureText(statusText, 30);
        DrawText(statusText, 
                GetScreenWidth() / 2 - textWidth / 2, 
                500, 
                30, 
                statusColor);
    }
    
    Color ColorBrightness(Color color, float factor) {
        return Color{
            static_cast<unsigned char>(color.r + (255 - color.r) * factor),
            static_cast<unsigned char>(color.g + (255 - color.g) * factor),
            static_cast<unsigned char>(color.b + (255 - color.b) * factor),
            color.a
        };
    }
};

int main() {
    const int screenWidth = 960;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Countdown Timer - Raylib");
    SetTargetFPS(60);
    
    CountdownTimer timer;
    
    while (!WindowShouldClose()) {
        timer.Update();
        
        BeginDrawing();
        timer.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}