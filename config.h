#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ====== DISPLAY CONFIGURATION ======
// Waveshare ESP32-S3-Touch-AMOLED-1.8 pins (QSPI interface)
#define TFT_WIDTH 368
#define TFT_HEIGHT 448
#define TFT_ROTATION 0

// QSPI Pins for AMOLED display
#define QSPI_CLK 36
#define QSPI_D0 37
#define QSPI_D1 38
#define QSPI_D2 33
#define QSPI_D3 34
#define QSPI_CS 35

// Display chipset: SH8601 (V1) or CO5300 (V2)
// Uncomment based on your board version
#define DISPLAY_V2  // V2 with CO5300 driver
// #define DISPLAY_V1  // V1 with SH8601 driver

// ====== TOUCH CONFIGURATION ======
// Capacitive touch I2C pins
#define TOUCH_SDA 8
#define TOUCH_SCL 9
#define TOUCH_I2C_ADDR 0x20  // CST820 V2 / FT3168 V1
#define TOUCH_IRQ 3

// ====== AUDIO CONFIGURATION ======
// I2S Audio pins for ES8311 codec
#define I2S_MCLK 16
#define I2S_BCLK 7
#define I2S_WS 6
#define I2S_DOUT 45  // Speaker output
#define I2S_DIN 46   // Microphone input (optional)

// I2S Configuration
#define I2S_PORT 0
#define I2S_SAMPLE_RATE 44100
#define I2S_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define I2S_BUFFER_SIZE 4096

// Audio codec I2C address (ES8311)
#define AUDIO_CODEC_I2C_ADDR 0x18

// ====== SD CARD CONFIGURATION ======
// SDMMC pins for SD card
#define SD_CLK 12
#define SD_CMD 11
#define SD_D0 13
#define SD_D1 14
#define SD_D2 17
#define SD_D3 18

// Music folder path
#define MUSIC_FOLDER "/Music"
#define MAX_FILES 256
#define MAX_FILENAME_LENGTH 256

// ====== BUTTON CONFIGURATION ======
#define PWR_BUTTON 0
#define USER_BUTTON 21

// ====== BATTERY/POWER ======
// AXP2101 Power Management IC
#define BATTERY_ADC_PIN 4
#define BATTERY_ADC_CHANNEL ADC1_CHANNEL_3

// ====== UI CONFIGURATION ======
#define UI_REFRESH_RATE 30  // FPS
#define ANIMATION_SPEED 200  // ms
#define TOUCH_DEBOUNCE 50    // ms

// Color palette
#define COLOR_BG 0x000000          // Black
#define COLOR_ACCENT 0x00D4FF      // Cyan
#define COLOR_TEXT 0xFFFFFF         // White
#define COLOR_TEXT_DIM 0x808080     // Gray
#define COLOR_BUTTON 0x1E3A8A       // Dark blue
#define COLOR_BUTTON_PRESSED 0x00D4FF  // Cyan
#define COLOR_PROGRESS 0x00D4FF     // Cyan
#define COLOR_VOLUME_BG 0x333333    // Dark gray

// ====== DEBUG ======
#define DEBUG_SERIAL 1
#define SERIAL_BAUD 115200
#define DEBUG_LEVEL 1  // 0=off, 1=basic, 2=verbose

#if DEBUG_SERIAL
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(fmt, ...)
#endif

#endif // CONFIG_H
