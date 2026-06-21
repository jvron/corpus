#include <string>
#include <GLFW/glfw3.h>

#include "platform/clock.hpp"
#include "engine/world.hpp"

void Clock::init(World &world) {

    framesThisSecond = 0;
    
    double now = glfwGetTime();
    fpsLastTime = now;
    world.engineState.time.currentTime = now;
    world.engineState.time.previousTime = now;
};

void Clock::tick(World &world) {
    Time& time = world.engineState.time;
    
    double currentTime = glfwGetTime();

    time.currentTime = currentTime;
    time.deltaTime = time.currentTime - time.previousTime;
    time.previousTime = time.currentTime;

    time.frameCount++;

    framesThisSecond++;

    if (currentTime - fpsLastTime >= 1.0) {
        //std::cout << "\r FPS: " << std::to_string(fpsCounter) << std::flush;
        std::string title = world.engineConfig.windowConfig.title + std::string("   FPS: ") + std::to_string(framesThisSecond) ;
        glfwSetWindowTitle(world.engineState.windowState.handle, title.c_str());

        fpsLastTime = currentTime;
        framesThisSecond = 0;
    }
}