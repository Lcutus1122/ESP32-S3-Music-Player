/*
 * Waveshare ESP32-S3-Touch-AMOLED-1.8 Music Player
 * Polished audio player with SD card support and touchscreen UI
 *
 * Features:
 * - Play WAV/MP3 files from microSD card
 * - Touch-based UI on 1.8" AMOLED display
 * - Play/Pause/Next/Previous controls
 * - Volume control with visual feedback
 * - File browser for music selection
 * - Battery status display
 * - Progress bar with playback time
 */

#include "config.h"
#include "file_manager.h"
#include "audio_manager.h"
#include "ui.h"
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include <driver/i2s.h>

// ====== GLOBAL OBJECTS ======
FileManager fileManager;
AudioManager audioManager;
UI* uiManager = nullptr;

Arduino_DataBus* bus = nullptr;
Arduino_GFX* display = nullptr;

// ====== STATE VARIABLES ======
UI::UIState currentState = UI::STATE_PLAYER;
unsigned long lastUIUpdate = 0;
int selectedFileIndex = 0;
int browserScrollOffset = 0;
uint8_t currentBattery = 100;

// Touch coordinates
int lastTouchX = -1;
int lastTouchY = -1;
unsigned long lastTouchTime = 0;

// ====== FUNCTION PROTOTYPES ======
void initDisplay();
void initAudio();
void initFiles();
Void handleTouch();
void updateUI();
void drawPlayerUI();
void drawBrowserUI();
uint8_t getBatteryPercentage();

// ====== SETUP ======
void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    DEBUG_PRINTLN("\n=== ESP32-S3 Music Player Starting ===");
    
    // Initialize display
    DEBUG_PRINTLN("Initializing display...");
    initDisplay();
    
    // Initialize file system
    DEBUG_PRINTLN("Initializing SD card...");
    initFiles();
    
    // Initialize audio
    DEBUG_PRINTLN("Initializing audio...");
    initAudio();
    
    // Load initial music files
    if (fileManager.loadMusicFiles()) {
        DEBUG_PRINTLN("Music files loaded successfully");
        // Play first file automatically
        String firstFile = fileManager.getCurrentFile();
        if (!firstFile.isEmpty()) {
            audioManager.play(firstFile);
        }
    } else {
        DEBUG_PRINTLN("No music files found - enter browser mode");
        currentState = UI::STATE_BROWSER;
    }
    
    DEBUG_PRINTLN("Setup complete!");
}

// ====== MAIN LOOP ======
void loop() {
    // Update audio engine
    audioManager.loop();
    
    // Handle touch input
    handleTouch();
    
    // Update UI at fixed rate
    if (millis() - lastUIUpdate > (1000 / UI_REFRESH_RATE)) {
        updateUI();
        lastUIUpdate = millis();
    }
    
    delay(10);
}

// ====== INITIALIZATION FUNCTIONS ======

void initDisplay() {
    // Initialize I2C for touch
    Wire.begin(TOUCH_SDA, TOUCH_SCL, 400000);
    
    // Create QSPI data bus for AMOLED
    bus = new Arduino_ESP32QSPI(
        QSPI_CLK, QSPI_D0, QSPI_D1, QSPI_D2, QSPI_D3, QSPI_CS
    );
    
    // Create display object based on version
    #ifdef DISPLAY_V2
        // V2: CO5300 driver
        display = new Arduino_CO5300(bus);
    #else
        // V1: SH8601 driver
        display = new Arduino_SH8601(bus);
    #endif
    
    // Initialize and configure display
    display->begin();
    display->setRotation(TFT_ROTATION);
    display->fillScreen(COLOR_BG);
    
    // Create UI manager
    uiManager = new UI(display);
    uiManager->init();
    
    // Show splash screen
    display->setTextColor(COLOR_ACCENT);
    display->setFont(u8g2_font_5x7_tf);
    display->setCursor(TFT_WIDTH / 2 - 50, TFT_HEIGHT / 2 - 10);
    display->print("Music Player");
    display->setTextColor(COLOR_TEXT_DIM);
    display->setFont(u8g2_font_tom_thumb_4x6_tf);
    display->setCursor(TFT_WIDTH / 2 - 40, TFT_HEIGHT / 2 + 20);
    display->print("Loading...");
    delay(500);
}

void initAudio() {
    audioManager.init();
    DEBUG_PRINTLN("Audio initialized with I2S");
}

void initFiles() {
    if (!fileManager.init()) {
        DEBUG_PRINTLN("CRITICAL: SD card failed to initialize!");
        display->setTextColor(0xFF0000);  // Red
        display->setFont(u8g2_font_tom_thumb_4x6_tf);
        display->setCursor(10, TFT_HEIGHT / 2);
        display->print("SD Card Error!");
        delay(2000);
    }
}

// ====== TOUCH HANDLING ======

void handleTouch() {
    // Check if touch controller (CST820/FT3168) has data
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(0x02);  // Status register
    Wire.endTransmission();
    
    Wire.requestFrom(TOUCH_I2C_ADDR, 1);
    if (Wire.available()) {
        uint8_t touchPoints = Wire.read() & 0x0F;
        
        if (touchPoints > 0) {
            // Read touch coordinates
            Wire.beginTransmission(TOUCH_I2C_ADDR);
            Wire.write(0x03);  // X coordinate
            Wire.endTransmission();
            
            Wire.requestFrom(TOUCH_I2C_ADDR, 4);
            if (Wire.available() >= 4) {
                uint8_t x_h = Wire.read();
                uint8_t x_l = Wire.read();
                uint8_t y_h = Wire.read();
                uint8_t y_l = Wire.read();
                
                lastTouchX = ((x_h & 0x0F) << 8) | x_l;
                lastTouchY = ((y_h & 0x0F) << 8) | y_l;
                lastTouchTime = millis();
                
                DEBUG_PRINTF("Touch: X=%d, Y=%d\n", lastTouchX, lastTouchY);
                
                // Process touch based on current state
                if (currentState == UI::STATE_PLAYER) {
                    handlePlayerTouch(lastTouchX, lastTouchY);
                } else if (currentState == UI::STATE_BROWSER) {
                    handleBrowserTouch(lastTouchX, lastTouchY);
                }
            }
        }
    }
}

void handlePlayerTouch(int x, int y) {
    int buttonY = TFT_HEIGHT - 80;
    int buttonWidth = 50;
    int spacing = 15;
    int totalWidth = (buttonWidth * 3) + (spacing * 2);
    int startX = (TFT_WIDTH - totalWidth) / 2;
    
    // Previous button
    if (x > startX && x < startX + buttonWidth && y > buttonY && y < buttonY + 40) {
        if (fileManager.previousFile()) {
            String file = fileManager.getCurrentFile();
            audioManager.play(file);
            DEBUG_PRINTLN("Previous track");
        }
    }
    // Play/Pause button
    else if (x > startX + buttonWidth + spacing && x < startX + (buttonWidth * 2) + spacing && 
             y > buttonY && y < buttonY + 40) {
        audioManager.togglePlayPause();
        DEBUG_PRINTLN("Play/Pause toggled");
    }
    // Next button
    else if (x > startX + (buttonWidth + spacing) * 2 && x < startX + (buttonWidth * 3) + (spacing * 2) && 
             y > buttonY && y < buttonY + 40) {
        if (fileManager.nextFile()) {
            String file = fileManager.getCurrentFile();
            audioManager.play(file);
            DEBUG_PRINTLN("Next track");
        }
    }
    // Volume area (bottom)
    else if (y > TFT_HEIGHT - 30) {
        int volumePercent = (x * 21) / TFT_WIDTH;
        audioManager.setVolume(volumePercent);
        DEBUG_PRINTF("Volume: %d\n", volumePercent);
    }
    // Top area - open file browser
    else if (y < 50 && x < 100) {
        currentState = UI::STATE_BROWSER;
        browserScrollOffset = 0;
        selectedFileIndex = fileManager.getCurrentIndex();
        DEBUG_PRINTLN("Entering browser mode");
    }
}

void handleBrowserTouch(int x, int y) {
    // Back button or top area to return to player
    if (y < 30 || (x < 50 && y < 100)) {
        currentState = UI::STATE_PLAYER;
        DEBUG_PRINTLN("Returning to player");
        return;
    }
    
    // Item selection
    int itemHeight = 35;
    int maxItems = (TFT_HEIGHT - 40) / itemHeight;
    int startY = 25;
    
    for (int i = 0; i < maxItems; i++) {
        int fileIndex = i + browserScrollOffset;
        int itemY = startY + i * itemHeight;
        
        if (x > 5 && x < TFT_WIDTH - 5 && y > itemY - 15 && y < itemY + itemHeight - 5) {
            if (fileIndex < fileManager.getTotalFiles()) {
                fileManager.setCurrentFile(fileIndex);
                String file = fileManager.getCurrentFile();
                audioManager.play(file);
                currentState = UI::STATE_PLAYER;
                DEBUG_PRINTF("Playing: %s\n", file.c_str());
            }
            break;
        }
    }
}

// ====== UI UPDATE ======

void updateUI() {
    currentBattery = getBatteryPercentage();
    
    if (currentState == UI::STATE_PLAYER) {
        drawPlayerUI();
    } else if (currentState == UI::STATE_BROWSER) {
        drawBrowserUI();
    }
}

void drawPlayerUI() {
    String filename = fileManager.getCurrentFilename();
    bool isPlaying = audioManager.getIsPlaying();
    bool isPaused = audioManager.getIsPaused();
    uint8_t volume = audioManager.getVolume();
    int fileIndex = fileManager.getCurrentIndex();
    int totalFiles = fileManager.getTotalFiles();
    uint32_t currentTime = audioManager.getCurrentTime();
    uint32_t totalTime = 240000;  // Placeholder, ideally get from audio library
    
    uiManager->drawPlayerScreen(
        filename, isPlaying, isPaused, volume, fileIndex, totalFiles,
        currentTime, totalTime, currentBattery
    );
}

void drawBrowserUI() {
    std::vector<String> files;
    int count = 0;
    files = fileManager.getFileList(count);
    
    uiManager->drawBrowserScreen(files, selectedFileIndex, browserScrollOffset);
}

// ====== HELPER FUNCTIONS ======

uint8_t getBatteryPercentage() {
    // Read battery ADC
    uint16_t raw = analogRead(BATTERY_ADC_PIN);
    
    // Convert to voltage (assuming 12-bit ADC, 3.3V reference)
    // Battery voltage is typically 3.0V (0%) to 4.2V (100%)
    float voltage = (raw / 4095.0) * 3.3 * 1.1;  // 1.1 factor for voltage divider
    
    // Convert voltage to percentage
    uint8_t percent = constrain((int)((voltage - 3.0) / 1.2 * 100), 0, 100);
    
    return percent;
}

// Audio info callback for debugging
void audio_info(const char* info) {
    DEBUG_PRINTF("Audio Info: %s\n", info);
}

// Optional: Emergency stop via power button
void checkPowerButton() {
    if (digitalRead(PWR_BUTTON) == LOW) {
        delay(50);  // Debounce
        if (digitalRead(PWR_BUTTON) == LOW) {
            audioManager.stop();
            display->fillScreen(COLOR_BG);
            display->setTextColor(COLOR_TEXT);
            display->setFont(u8g2_font_tom_thumb_4x6_tf);
            display->setCursor(10, TFT_HEIGHT / 2);
            display->print("Powering off...");
            delay(1000);
            // Implement actual power-off here if needed
        }
    }
}
