// include/DebugUtils.hpp
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

class GameLogger {
public:
    static GameLogger& instance() {
        static GameLogger logger;
        return logger;
    }

    template<typename T>
    void log(const T& message) {
        if (!logFile.is_open()) return;
        
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        std::string timestamp = std::string(std::ctime(&now_c));

        // remove trailing whitespace
        timestamp.resize(timestamp.find_last_not_of(" \n\r\t") + 1);

        ss << timestamp << ": " << message << std::endl;
        
        logFile << ss.str();
        logFile.flush();
    }

private:
    GameLogger() {
        logFile.open("game_debug.log", std::ios::out | std::ios::app);
    }
    
    ~GameLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    std::ofstream logFile;
};

#define LOG(msg) GameLogger::instance().log(msg)
#define LOG_VALUE(name, value) GameLogger::instance().log(std::string(name) + ": " + std::to_string(value))
#define LOG_VECTOR(name, vec) GameLogger::instance().log(std::string(name) + ": size = " + std::to_string(vec.size()))
#define ASSERT_LOG(condition, msg) \
    if (!(condition)) { \
        LOG("Assertion failed: " + std::string(msg)); \
        std::cerr << "Assertion failed: " << msg << std::endl; \
    }
#define GOT_HERE() LOG(std::string(__FILE__) + ":" + std::to_string(__LINE__))
