#pragma once 

struct World;

namespace Clock {

    double getRawTime();
    double getFrameTime(World& world);

    void init(World& world);
    void tick(World& world);

}