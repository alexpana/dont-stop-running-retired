#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <ctime>
#include <string.h>
#include <time.h>

namespace engine {
    class Log {
    public:

        Log(const std::string &className) : className(className) {

        }

        std::ostream &debug() {
            return prepareOutputStream("DEBUG");

        }

        std::ostream &info() {
            return prepareOutputStream("INFO");
        }

        std::ostream &error() {
            return prepareOutputStream("ERROR");
        }

        std::ostream &warning() {
            return prepareOutputStream("WARNING");
        }

    private:
        std::ostream &prepareOutputStream(const std::string &level) {
            char timestamp[10];

            memset(timestamp, 0, sizeof(timestamp));

            time_t now = time(nullptr);
            tm tm = *localtime(&now);

            strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &tm);

            return std::cout << timestamp << " " << level << " [" << className << "] ";
        }

        std::string className;
    };
}