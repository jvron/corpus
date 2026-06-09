#pragma once

#include "engine/world.hpp"

namespace Window {

    void create(World &world);
    bool shouldClose(World &world);
    void pollEvents(World &world);
    void swapBuffers(World &world);
    void destroy(World &world);
};