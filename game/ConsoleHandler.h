#pragma once

#include <thread>

#include "World.h"

class ConsoleHandler {
public:
    ConsoleHandler(World *world) {
        // create input thread
        std::thread{[world]() {
            while (true) {
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
};
