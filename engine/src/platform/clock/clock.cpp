#include <GLFW/glfw3.h>
#include <string>

#include "platform/clock.hpp"
#include "engine/world.hpp"

double Clock::getRawTime() {
    return glfwGetTime();
}

double Clock::getFrameTime(World &world) {
    return world.engineState.time.currentTime;
}

void Clock::init(World &world) {
    
    double now = getRawTime();

    world.engineState.time.fpsTimer = now;
    world.engineState.time.currentTime = now;
    world.engineState.time.previousTime = now;
};

void Clock::tick(World &world) {
    Time& time = world.engineState.time;

    double now = getRawTime();
    
    time.currentTime = now;

    time.deltaTime = time.currentTime - time.previousTime;
    time.previousTime = time.currentTime;

    time.frameCount++;

    time.framesThisSecond++;

    if (now - time.fpsTimer >= 1.0) {

        std::string title = world.engineConfig.windowConfig.title + "   FPS: " + std::to_string(time.framesThisSecond) ;
        glfwSetWindowTitle(world.engineState.windowState.handle, title.c_str());

        time.fpsTimer = now;
        time.framesThisSecond = 0;
    }
}
