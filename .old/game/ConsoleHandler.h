#pragma once

class World;

class ConsoleHandler {
public:
    ConsoleHandler(World *world);

    void start();

    void stop();

private:
    bool started = false;
    bool stopped = false;
};
