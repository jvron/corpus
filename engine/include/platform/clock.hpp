#pragma once 

struct World;

namespace Clock {

    static int framesThisSecond;
    static double fpsLastTime;

    void init(World& world);
    void tick(World& world);
}