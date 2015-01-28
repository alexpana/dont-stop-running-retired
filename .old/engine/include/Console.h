#pragma once

#include <functional>
#include <unordered_map>

namespace engine {
    class Console {
        typedef std::function<void(const std::string &)> Callback;

    public:
        Console() {
        }

        void registerCallback(const std::string &name, const Callback &callback) {
            callbacks[name] = callback;
        }

        void parseInput(const std::string &input) {
            // take the command part of the input
            // resolve to the callback
            // call the callback with the rest of the input
        }

    private:
        std::unordered_map<std::string, Callback> callbacks;

    };
}
