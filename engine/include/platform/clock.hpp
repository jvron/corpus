#pragma once 

struct World;

namespace Clock {

    void init(World& world);
    void tick(World& world);
}