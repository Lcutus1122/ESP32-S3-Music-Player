#ifndef UI_H
#define UI_H

#include "config.h"
#include <Arduino_GFX_Library.h>
#include <vector>

class UI {
private:
    Arduino_GFX* display;
    uint16_t screenWidth;
    uint16_t screenHeight;

public:
    enum UIState {
        STATE_PLAYER,
        STATE_BROWSER,
        STATE_VOLUME,
        STATE_SETTINGS
    };

    struct Button {
        int16_t x, y, w, h;
        String label;
        bool pressed = false;
        uint16_t color = COLOR_BUTTON;
        uint16_t textColor = COLOR_TEXT;
    };

    UI(Arduino_GFX* gfx) : display(gfx) {
        screenWidth = gfx->width();
        screenHeight = gfx->height();
    }

    // Initialize display
    void init() {
        display->begin();
        display->fillScreen(COLOR_BG);
        DEBUG_PRINTLN("Display initialized");
    }

    // Clear screen
    void clear() {
        display->fillScreen(COLOR_BG);
    }

    // Draw player screen
    void drawPlayerScreen(const String& filename, bool isPlaying, bool isPaused,
                         uint8_t volume, int fileIndex, int totalFiles,
                         uint32_t currentTime, uint32_t totalTime, uint8_t battery) {
        clear();

        // Header
        display->setTextColor(COLOR_ACCENT);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(10, 15);
        display->print("Now Playing");

        // Battery indicator (top right)
        drawBatteryIndicator(screenWidth - 40, 5, battery);

        // Album art placeholder (circle)
        int artSize = 120;
        int artX = (screenWidth - artSize) / 2;
        int artY = 35;
        display->drawCircle(artX + artSize / 2, artY + artSize / 2, artSize / 2, COLOR_ACCENT);
        display->fillCircle(artX + artSize / 2, artY + artSize / 2, artSize / 2 - 2, COLOR_BUTTON);

        // Music note icon inside circle
        drawMusicNote(artX + artSize / 2 - 10, artY + artSize / 2 - 10);

        // Filename
        display->setTextColor(COLOR_TEXT);
        display->setFont(u8g2_font_5x7_tf);
        int filenameY = artY + artSize + 20;
        drawTextCentered(filename, screenWidth / 2, filenameY, 30);

        // File counter
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        String counter = String(fileIndex + 1) + "/" + String(totalFiles);
        drawTextCentered(counter, screenWidth / 2, filenameY + 20, 30);

        // Progress bar
        drawProgressBar(20, filenameY + 35, screenWidth - 40, 6, currentTime, totalTime);

        // Time display
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        String timeStr = formatTime(currentTime) + " / " + formatTime(totalTime);
        drawTextCentered(timeStr, screenWidth / 2, filenameY + 50, 30);

        // Control buttons
        int buttonY = screenHeight - 80;
        drawControlButtons(buttonY, isPlaying, isPaused);

        // Volume bar at bottom
        drawVolumeBar(20, screenHeight - 25, screenWidth - 40, 8, volume);

        // Footer menu
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(10, screenHeight - 5);
        display->print("[Files] [Vol] [Menu]");
    }

    // Draw file browser screen
    void drawBrowserScreen(const std::vector<String>& files, int selectedIndex, int scrollOffset) {
        clear();

        // Header
        display->setTextColor(COLOR_ACCENT);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(10, 15);
        display->print("Music Files");

        // List items
        int itemHeight = 35;
        int maxItems = (screenHeight - 40) / itemHeight;
        int startY = 25;

        display->setFont(u8g2_font_5x7_tf);

        for (int i = 0; i < maxItems && i + scrollOffset < files.size(); i++) {
            int fileIndex = i + scrollOffset;
            String filename = files[fileIndex];

            // Extract just the filename
            int lastSlash = filename.lastIndexOf('/');
            if (lastSlash != -1) {
                filename = filename.substring(lastSlash + 1);
            }

            int itemY = startY + i * itemHeight;

            // Highlight selected
            if (fileIndex == selectedIndex) {
                display->fillRect(5, itemY - 15, screenWidth - 10, itemHeight - 5, COLOR_BUTTON);
                display->setTextColor(COLOR_BUTTON_PRESSED);
            } else {
                display->setTextColor(COLOR_TEXT);
            }

            display->setCursor(15, itemY);
            display->print(filename.substring(0, 30));
        }

        // Footer
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        String footerStr = String(selectedIndex + 1) + "/" + String(files.size()) + " | Select to play";
        drawTextCentered(footerStr, screenWidth / 2, screenHeight - 10, screenWidth - 20);
    }

    // Draw volume control screen
    void drawVolumeScreen(uint8_t volume) {
        clear();

        display->setTextColor(COLOR_ACCENT);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(screenWidth / 2 - 30, 50);
        display->print("Volume");

        // Large volume slider
        int sliderWidth = screenWidth - 60;
        int sliderX = 30;
        int sliderY = 150;

        display->drawRect(sliderX, sliderY, sliderWidth, 20, COLOR_ACCENT);
        int filledWidth = (volume * sliderWidth) / 21;
        display->fillRect(sliderX, sliderY, filledWidth, 20, COLOR_ACCENT);

        // Volume value
        display->setTextColor(COLOR_TEXT);
        display->setFont(u8g2_font_5x7_tf);
        String volStr = String(volume);
        drawTextCentered(volStr, screenWidth / 2, 200, 30);

        // Instructions
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(20, screenHeight - 20);
        display->print("Use +/- buttons");
    }

    // Draw settings screen
    void drawSettingsScreen(const String& version, bool autoPlay) {
        clear();

        display->setTextColor(COLOR_ACCENT);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(10, 20);
        display->print("Settings");

        display->setTextColor(COLOR_TEXT);
        display->setFont(u8g2_font_5x7_tf);
        display->setCursor(20, 60);
        display->print("Version: ");
        display->print(version);

        display->setCursor(20, 100);
        display->print("Auto-play: ");
        display->print(autoPlay ? "ON" : "OFF");

        // Back instruction
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(20, screenHeight - 20);
        display->print("Press back to exit");
    }

private:
    // Helper: Draw text centered
    void drawTextCentered(const String& text, int x, int y, int maxWidth) {
        int len = text.length();
        int charWidth = 6;
        int textWidth = len * charWidth;
        int startX = constrain(x - textWidth / 2, 5, screenWidth - textWidth - 5);
        display->setCursor(startX, y);
        display->print(text);
    }

    // Helper: Draw progress bar
    void drawProgressBar(int x, int y, int width, int height, uint32_t current, uint32_t total) {
        display->drawRect(x, y, width, height, COLOR_TEXT_DIM);
        if (total > 0) {
            int filledWidth = (current * width) / total;
            display->fillRect(x, y, filledWidth, height, COLOR_PROGRESS);
        }
    }

    // Helper: Draw volume bar
    void drawVolumeBar(int x, int y, int width, int height, uint8_t volume) {
        display->drawRect(x, y, width, height, COLOR_TEXT_DIM);
        int filledWidth = (volume * width) / 21;
        display->fillRect(x, y, filledWidth, height, COLOR_VOLUME_BG);
    }

    // Helper: Draw battery indicator
    void drawBatteryIndicator(int x, int y, uint8_t percent) {
        display->drawRect(x, y, 30, 12, COLOR_TEXT_DIM);
        display->fillRect(x + 31, y + 3, 2, 6, COLOR_TEXT_DIM);
        int filledWidth = (percent * 28) / 100;
        if (filledWidth > 0) {
            uint16_t color = percent > 20 ? COLOR_ACCENT : 0xFF00;  // Red if low
            display->fillRect(x + 1, y + 1, filledWidth, 10, color);
        }
        display->setTextColor(COLOR_TEXT_DIM);
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(x + 8, y + 10);
        display->print(percent);
    }

    // Helper: Draw music note icon
    void drawMusicNote(int x, int y) {
        display->drawLine(x + 5, y, x + 5, y + 15, COLOR_TEXT);
        display->fillCircle(x + 5, y + 18, 3, COLOR_TEXT);
        display->drawLine(x + 8, y + 2, x + 8, y + 12, COLOR_TEXT);
        display->fillCircle(x + 8, y + 15, 3, COLOR_TEXT);
    }

    // Helper: Draw control buttons (play, prev, next)
    void drawControlButtons(int y, bool isPlaying, bool isPaused) {
        int buttonWidth = 50;
        int buttonHeight = 40;
        int spacing = 15;
        int totalWidth = (buttonWidth * 3) + (spacing * 2);
        int startX = (screenWidth - totalWidth) / 2;

        // Previous button
        drawButton(startX, y, buttonWidth, buttonHeight, "<", COLOR_BUTTON);

        // Play/Pause button
        String playLabel = (isPlaying && !isPaused) ? "||" : "▶";
        drawButton(startX + buttonWidth + spacing, y, buttonWidth, buttonHeight, playLabel, COLOR_BUTTON);

        // Next button
        drawButton(startX + (buttonWidth + spacing) * 2, y, buttonWidth, buttonHeight, ">", COLOR_BUTTON);
    }

    // Helper: Draw button
    void drawButton(int x, int y, int w, int h, const String& label, uint16_t color) {
        display->drawRect(x, y, w, h, color);
        display->setTextColor(COLOR_TEXT);
        display->setFont(u8g2_font_5x7_tf);
        int textX = x + w / 2 - 5;
        int textY = y + h / 2 + 3;
        display->setCursor(textX, textY);
        display->print(label);
    }

    // Helper: Format time MM:SS
    String formatTime(uint32_t ms) {
        uint32_t seconds = ms / 1000;
        uint32_t minutes = seconds / 60;
        uint32_t secs = seconds % 60;
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", (int)minutes, (int)secs);
        return String(timeStr);
    }
};

#endif // UI_H
