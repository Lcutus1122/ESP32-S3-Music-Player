# ESP32-S3 Touch AMOLED Music Player

A polished music player application for the **Waveshare ESP32-S3-Touch-AMOLED-1.8** with SD card support, touchscreen controls, and elegant UI.

## Features

- 🎵 **Audio Playback**: Play WAV files from microSD card via built-in speaker
- 📱 **Touch UI**: Responsive touchscreen controls on 1.8" AMOLED display (368×448)
- 📂 **File Browser**: Navigate and select music files from SD card
- ⏯️ **Playback Controls**: Play, pause, next, previous, volume control
- 🔋 **Battery Display**: Shows battery status and charging state
- 🎚️ **Progress Bar**: Visual feedback during playback
- 🌙 **Elegant Design**: Modern UI with smooth animations

## Hardware Requirements

- **Waveshare ESP32-S3-Touch-AMOLED-1.8** board
- **MicroSD card** with music files (WAV format recommended)
- **USB-C cable** for programming and power

### Built-in Components

- Display: 1.8" AMOLED (368×448 resolution)
- Audio Codec: ES8311
- Speaker: Built-in SMD speaker with amplifier
- Storage: TF card slot
- Sensors: 6-axis IMU (QMI8658), RTC (PCF85063)
- Connectivity: Wi-Fi 2.4GHz, Bluetooth 5.0 LE
- Power Management: AXP2101 (3.7V Li-ion battery support)

## Software Setup

### Required Libraries

Install via Arduino Library Manager:

1. **Arduino_GFX** (by Moon On Our Nation)
   - For AMOLED display rendering
   
2. **ESP32-audioI2S** (by schreibfaul1)
   - For audio decoding and playback
   
3. **SD** (built-in with ESP32 core)
   - For SD card file system access

### Installation Steps

1. **Install Arduino IDE** (v1.8.19 or later)

2. **Add ESP32 Board Support**:
   - Preferences → Additional Board Manager URLs
   - Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Boards Manager → Search "ESP32" → Install "esp32 by Espressif Systems"

3. **Install Required Libraries**:
   - Sketch → Include Library → Manage Libraries
   - Search and install:
     - "Arduino_GFX"
     - "ESP32-audioI2S"

4. **Board Configuration**:
   - Select Board: `ESP32-S3 Dev Module`
   - CPU Frequency: 240MHz
   - Flash Size: 16MB
   - Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)
   - USB CDC On Boot: Enabled
   - Upload Speed: 921600

5. **Upload the Sketch**:
   - Connect via USB-C
   - Select the correct COM port
   - Click Upload

## File Structure

```
ESP32-S3-Music-Player/
├── ESP32_S3_MusicPlayer.ino      # Main application sketch
├── ui.h                          # UI rendering functions
├── audio_manager.h               # Audio playback management
├── file_manager.h                # SD card file operations
├── config.h                      # Hardware configuration & pins
└── README.md                     # Documentation
```

## SD Card Setup

1. **Format** your microSD card as **FAT32**
2. **Create a folder** named `Music` at the root
3. **Add WAV files** (44.1kHz, 16-bit mono or stereo recommended)
   ```
   /Music
     ├── song1.wav
     ├── song2.wav
     └── song3.wav
   ```

## Usage

### Touch Controls

- **Play/Pause**: Tap the play button in the center
- **Next Track**: Swipe right or tap next arrow
- **Previous Track**: Swipe left or tap previous arrow
- **Volume**: Drag the volume slider or tap +/- buttons
- **File Browser**: Tap the "Files" button to browse SD card
- **Select Track**: Tap any file in the browser list

### Serial Monitor

- Open Serial Monitor (115200 baud) to see debug information
- Battery percentage, file info, and playback status displayed

## Pinout Configuration

**Display & Touch (QSPI + I2C)**
- QSPI_CLK: GPIO 36
- QSPI_D0: GPIO 37
- QSPI_D1: GPIO 38
- QSPI_D2: GPIO 33
- QSPI_D3: GPIO 34
- QSPI_CS: GPIO 35
- Touch I2C SDA: GPIO 8
- Touch I2C SCL: GPIO 9

**Audio (I2S)**
- I2S MCLK: GPIO 16
- I2S BCLK: GPIO 7
- I2S WS: GPIO 6
- I2S DOUT: GPIO 45

**SD Card (SDMMC)**
- SD CLK: GPIO 12
- SD CMD: GPIO 11
- SD D0: GPIO 13
- SD D1: GPIO 14
- SD D2: GPIO 17
- SD D3: GPIO 18

**Other**
- Power Button: GPIO 0
- User Button: GPIO 21
- Battery ADC: GPIO 4

## Supported Audio Formats

- **WAV** (recommended): 8-48 kHz, mono/stereo, 8-16 bit
- **MP3**: With additional decoding library
- **AAC/FLAC**: Supported with extended configuration

## Tips & Troubleshooting

### No Sound
- Check SD card is properly inserted
- Verify WAV files are in `/Music` folder
- Increase volume in app or with volume slider
- Check USB-C connection (power is essential)

### Display Issues
- Verify V1/V2 board version (check label on back)
- Ensure Arduino_GFX library is up to date
- Try the display examples from Waveshare GitHub

### SD Card Not Found
- Format card as FAT32
- Try a different microSD card
- Check SDMMC pin connections if custom wiring

### Audio Playback Errors
- Use 44.1kHz 16-bit WAV files
- Ensure file names don't have special characters
- Check file isn't corrupted (try in media player)

### Touch Unresponsive
- Calibrate touch (see code comments)
- Check I2C connection for touch controller
- Board version mismatch (V1 vs V2)?

## Performance Optimization

- **Buffer Size**: Adjust in `config.h` for audio quality vs. memory
- **UI Refresh Rate**: 30 FPS by default, adjustable
- **PSRAM**: Uses 8MB PSRAM for efficient buffer management

## References

- [Waveshare ESP32-S3-Touch-AMOLED-1.8 Official Docs](https://docs.waveshare.com/ESP32-S3-Touch-AMOLED-1.8)
- [Arduino_GFX GitHub](https://github.com/moononournation/Arduino_GFX)
- [ESP32-audioI2S GitHub](https://github.com/schreibfaul1/ESP32-audioI2S)
- [Waveshare GitHub Repo](https://github.com/waveshareteam/ESP32-S3-Touch-AMOLED-1.8)

## License

MIT License - Feel free to modify and share!

## Contributing

Issues and pull requests welcome. Please test thoroughly on your hardware before submitting.
