#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ====== DISPLAY CONFIGURATION ======
// Waveshare ESP32-S3-Touch-AMOLED-1.8 V1 hardware
#define TFT_WIDTH 368
#define TFT_HEIGHT 448
#define TFT_ROTATION 0

// QSPI pins for AMOLED display on V1 board
#define QSPI_CLK 36
#define QSPI_D0 37
#define QSPI_D1 38
#define QSPI_D2 33
#define QSPI_D3 34
#define QSPI_CS 35

// V1 board uses SH8601 AMOLED display driver
#define DISPLAY_V1
// #define DISPLAY_V2

// ====== TOUCH CONFIGURATION ======
// V1 boards use FT3168 capacitive touch controller over I2C
#define TOUCH_SDA 8
#define TOUCH_SCL 9
#define TOUCH_I2C_ADDR 0x38
#define TOUCH_IRQ 3
#define TOUCH_RST 48

// ====== AUDIO CONFIGURATION ======
// ES8311 audio codec on the board
#define I2S_MCLK 16
#define I2S_BCLK 7
#define I2S_WS 6
#define I2S_DOUT 45
#define I2S_DIN 46

#define I2S_PORT 0
#define I2S_SAMPLE_RATE 44100
#define I2S_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define I2S_BUFFER_SIZE 4096

#define AUDIO_CODEC_I2C_ADDR 0x18
#define AUDIO_CODEC_SDA 8
#define AUDIO_CODEC_SCL 9

// ====== SD CARD CONFIGURATION ======
// SDMMC pins for the onboard microSD slot
#define SD_CLK 12
#define SD_CMD 11
#define SD_D0 13
#define SD_D1 14
#define SD_D2 17
#define SD_D3 18

#define MUSIC_FOLDER "/Music"
#define MAX_FILES 256
#define MAX_FILENAME_LENGTH 256

// ====== BUTTONS ======
#define PWR_BUTTON 0
#define USER_BUTTON 21

// ====== BATTERY / POWER ======
#define BATTERY_ADC_PIN 4
#define BATTERY_ADC_CHANNEL ADC1_CHANNEL_3
#define AXP_I2C_ADDR 0x34
#define AXP_SDA 8
#define AXP_SCL 9

// ====== UI ======
#define UI_REFRESH_RATE 30
#define ANIMATION_SPEED 200
#define TOUCH_DEBOUNCE 50

// Color palette
#define COLOR_BG 0x000000
#define COLOR_ACCENT 0x00D4FF
#define COLOR_TEXT 0xFFFFFF
#define COLOR_TEXT_DIM 0x808080
#define COLOR_BUTTON 0x1E3A8A
#define COLOR_BUTTON_PRESSED 0x00D4FF
#define COLOR_PROGRESS 0x00D4FF
#define COLOR_VOLUME_BG 0x333333

// ====== DEBUG ======
#define DEBUG_SERIAL 1
#define SERIAL_BAUD 115200
#define DEBUG_LEVEL 1

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
