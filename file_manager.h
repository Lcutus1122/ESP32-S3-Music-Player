#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "config.h"
#include "FS.h"
#include "SD_MMC.h"
#include <vector>

class FileManager {
private:
    std::vector<String> playlist;
    int currentFileIndex = 0;
    String currentPath = MUSIC_FOLDER;

public:
    FileManager() {}

    // Initialize SD card
    bool init() {
        if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_HIGHSPEED)) {
            DEBUG_PRINTLN("SD Card Mount Failed");
            return false;
        }
        DEBUG_PRINTLN("SD Card Initialized");
        return true;
    }

    // Load music files from directory
    bool loadMusicFiles(const String& path = MUSIC_FOLDER) {
        playlist.clear();
        currentFileIndex = 0;
        currentPath = path;

        File dir = SD_MMC.open(path);
        if (!dir || !dir.isDirectory()) {
            DEBUG_PRINTLN("Failed to open directory");
            return false;
        }

        File file = dir.openNextFile();
        while (file) {
            if (!file.isDirectory()) {
                String filename = file.name();
                // Check for audio file extensions
                if (filename.endsWith(".wav") || filename.endsWith(".WAV") ||
                    filename.endsWith(".mp3") || filename.endsWith(".MP3")) {
                    playlist.push_back(path + "/" + filename);
                    DEBUG_PRINTF("Found: %s\n", filename.c_str());
                }
            }
            file = dir.openNextFile();
        }
        dir.close();

        if (playlist.size() == 0) {
            DEBUG_PRINTLN("No audio files found");
            return false;
        }

        DEBUG_PRINTF("Loaded %d audio files\n", playlist.size());
        return true;
    }

    // Get file list for browser
    std::vector<String> getFileList(int& count) {
        count = playlist.size();
        return playlist;
    }

    // Get current playing file
    String getCurrentFile() {
        if (currentFileIndex < playlist.size()) {
            return playlist[currentFileIndex];
        }
        return "";
    }

    // Get filename only (without path)
    String getCurrentFilename() {
        String file = getCurrentFile();
        int lastSlash = file.lastIndexOf('/');
        if (lastSlash != -1) {
            return file.substring(lastSlash + 1);
        }
        return file;
    }

    // Get file at index
    String getFileAt(int index) {
        if (index >= 0 && index < playlist.size()) {
            return playlist[index];
        }
        return "";
    }

    // Get filename at index
    String getFilenameAt(int index) {
        String file = getFileAt(index);
        int lastSlash = file.lastIndexOf('/');
        if (lastSlash != -1) {
            return file.substring(lastSlash + 1);
        }
        return file;
    }

    // Next file
    bool nextFile() {
        if (playlist.size() > 0) {
            currentFileIndex = (currentFileIndex + 1) % playlist.size();
            return true;
        }
        return false;
    }

    // Previous file
    bool previousFile() {
        if (playlist.size() > 0) {
            currentFileIndex = (currentFileIndex - 1 + playlist.size()) % playlist.size();
            return true;
        }
        return false;
    }

    // Set current file by index
    bool setCurrentFile(int index) {
        if (index >= 0 && index < playlist.size()) {
            currentFileIndex = index;
            return true;
        }
        return false;
    }

    // Get current index
    int getCurrentIndex() {
        return currentFileIndex;
    }

    // Get total files
    int getTotalFiles() {
        return playlist.size();
    }

    // File exists check
    bool fileExists(const String& path) {
        File file = SD_MMC.open(path);
        bool exists = file;
        file.close();
        return exists;
    }

    // Get file size
    uint32_t getFileSize(const String& path) {
        File file = SD_MMC.open(path);
        if (!file) return 0;
        uint32_t size = file.size();
        file.close();
        return size;
    }
};

#endif // FILE_MANAGER_H
