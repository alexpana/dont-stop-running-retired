#include "ConsoleHandler.h"

#include <iostream>
#include <thread>

#include "World.h"

ConsoleHandler::ConsoleHandler(World *world) {
    std::thread{[&]() {
        // only start working after the started flag has been set;
        while (!started) {
            std::this_thread::yield();
        }

        // process input while not stopped
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

void ConsoleHandler::start() {
    started = true;
}

void ConsoleHandler::stop() {
    stopped = true;
}
