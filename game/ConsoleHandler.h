#pragma once

#include <thread>

#include "World.h"

class ConsoleHandler {
public:
    ConsoleHandler(World *world) {
        std::thread{[&]() {
            while (!started) {
                std::this_thread::yield();
            }

            while (!stopped) {
                std::string cmd;
                int n;
                std::cin >> cmd;

                if (cmd == "setv") {
                    std::cin >> n;
                    world->getRunner()->setSpeed(n);
                }

                if (cmd == "setg") {
                    std::cin >> n;
                    world->getRunner()->setGravity(n);
                }

                if (cmd == "setj") {
                    std::cin >> n;
                    world->getRunner()->setJumpVelocity(n);
                }

                if (cmd == "p") {
                    world->displayConstants();
                }
            }
        }}.detach();
    }

    void start() {
        started = true;
    }

    void stop() {
        stopped = true;
    }

private:
    bool started = false;
    bool stopped = false;
};
