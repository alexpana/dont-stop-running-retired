#pragma once

#include <iostream>
#include <string>

namespace engine {
    class Log {
    public:
        Log &log(std::string message) {
            std::cout << message << std::endl;
            return *this;
        }

//    Log& log(std::string method, std::string message)//    {
//        std::cout << "[" << method << "] " << message << std::endl;
//        return *this;
//    }

        Log &log(const char *fmt, const char *...) {
            return *this;
        }
    };
}