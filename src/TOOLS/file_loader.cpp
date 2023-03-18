#include <iostream>
#include <string>
#include <filesystem>
#include "file_loader.h"

namespace FileSystem {
    std::string getPath(const std::string& filename) {
        // Get the path to the executable
        std::filesystem::path exePath = std::filesystem::current_path();
        // Append the filename to the path
        exePath /= filename;
        // Convert the path to a string and return it
        return exePath.string();
    }
}
