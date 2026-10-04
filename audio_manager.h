#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "config.h"
#include "Audio.h"

class AudioManager {
private:
    Audio* audio;
    bool isPlaying = false;
    bool isPaused = false;
    uint8_t volume = 15;  // 0-21
    unsigned long playbackStartTime = 0;
    unsigned long pausedTime = 0;

public:
    AudioManager() {
        audio = new Audio();
    }

    ~AudioManager() {
        if (audio) delete audio;
    }

    // Initialize audio system
    bool init() {
        // Set I2S pins
        audio->setPinout(I2S_BCLK, I2S_WS, I2S_DOUT, I2S_DIN, I2S_MCLK);
        
        // Set initial volume (0-21)
        audio->setVolume(volume);
        
        DEBUG_PRINTLN("Audio system initialized");
        return true;
    }

    // Play file
    bool play(const String& filePath) {
        if (filePath.isEmpty()) {
            DEBUG_PRINTLN("Invalid file path");
            return false;
        }

        stop();
        
        DEBUG_PRINTF("Playing: %s\n", filePath.c_str());
        
        // Connect to file system
        if (!audio->connecttoFS(SD_MMC, filePath.c_str())) {
            DEBUG_PRINTLN("Failed to connect to file");
            return false;
        }
        
        isPlaying = true;
        isPaused = false;
        playbackStartTime = millis();
        return true;
    }

    // Pause playback
    bool pause() {
        if (isPlaying && !isPaused) {
            audio->pauseResume(true);
            isPaused = true;
            pausedTime = millis();
            DEBUG_PRINTLN("Playback paused");
            return true;
        }
        return false;
    }

    // Resume playback
    bool resume() {
        if (isPlaying && isPaused) {
            audio->pauseResume(false);
            isPaused = false;
            playbackStartTime = millis() - (pausedTime - playbackStartTime);
            DEBUG_PRINTLN("Playback resumed");
            return true;
        }
        return false;
    }

    // Stop playback
    void stop() {
        if (isPlaying) {
            audio->stopSong();
            isPlaying = false;
            isPaused = false;
            playbackStartTime = 0;
            pausedTime = 0;
            DEBUG_PRINTLN("Playback stopped");
        }
    }

    // Play/Pause toggle
    void togglePlayPause() {
        if (isPlaying && !isPaused) {
            pause();
        } else if (isPaused) {
            resume();
        }
    }

    // Set volume (0-21)
    void setVolume(uint8_t vol) {
        volume = constrain(vol, 0, 21);
        audio->setVolume(volume);
        DEBUG_PRINTF("Volume: %d\n", volume);
    }

    // Get volume
    uint8_t getVolume() {
        return volume;
    }

    // Volume up
    void volumeUp() {
        setVolume(volume + 1);
    }

    // Volume down
    void volumeDown() {
        if (volume > 0) {
            setVolume(volume - 1);
        }
    }

    // Get playback state
    bool getIsPlaying() {
        return isPlaying;
    }

    // Get pause state
    bool getIsPaused() {
        return isPaused;
    }

    // Get current playback time in ms
    uint32_t getCurrentTime() {
        if (!isPlaying) return 0;
        if (isPaused) return pausedTime - playbackStartTime;
        return millis() - playbackStartTime;
    }

    // Update audio engine (must be called regularly in loop)
    void loop() {
        if (audio) {
            audio->loop();
        }
    }

    // Audio info callback (for debug)
    static void audioInfo(const char *info) {
        if (DEBUG_LEVEL > 1) {
            DEBUG_PRINTF("Audio: %s\n", info);
        }
    }
};

#endif // AUDIO_MANAGER_H
