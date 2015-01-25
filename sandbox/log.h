#pragma once

#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <cstring>

namespace dsr {
    class Log {
    public:
        Log(const std::string &className) : className(className) {
        }

        std::ostream &debug() {
            return prepareOutputStream("DEBUG");

        }

        std::ostream &info() {
            return prepareOutputStream("INFO ");
        }

        std::ostream &error() {
            return prepareOutputStream("ERROR");
        }

        std::ostream &warning() {
            return prepareOutputStream("WARN ");
        }

        template<typename T, typename... Ts>
        void debug(T arg0, Ts... args) {
            auto &stream = prepareOutputStream("DEBUG");
            doLog(stream, arg0, args...);
            stream << std::endl;
        }

        template<typename T, typename... Ts>
        void info(T arg0, Ts... args) {
            auto &stream = prepareOutputStream("INFO ");
            doLog(stream, arg0, args...);
            stream << std::endl;
        }

        template<typename T, typename... Ts>
        void warn(T arg0, Ts... args) {
            auto &stream = prepareOutputStream("WARN ");
            doLog(stream, arg0, args...);
            stream << std::endl;
        }

        template<typename T, typename... Ts>
        void error(T arg0, Ts... args) {
            auto &stream = prepareOutputStream("ERROR");
            doLog(stream, arg0, args...);
            stream << std::endl;
        }

    private:
        template<typename T>
        void doLog(std::ostream &stream, T arg) {
            stream << arg;
        }

        template<typename T, typename... Ts>
        void doLog(std::ostream &stream, T arg0, Ts... args) {
            stream << arg0;
            doLog(stream, args...);
        }

        std::ostream &prepareOutputStream(const std::string &level) {
            char timestamp[10];

            memset(timestamp, 0, sizeof(timestamp));

            time_t now = time(nullptr);
            tm tm = *localtime(&now);

            strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &tm);

            return std::cout << timestamp << " " << level << " " << className << ": ";
        }

        std::string className;
    };
}