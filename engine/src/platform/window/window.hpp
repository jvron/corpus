#pragma once

#include "engine/registry.hpp"

namespace Window {

    void create(Registry &registry);
    bool shouldClose(Registry &registry);
    void pollEvents(Registry &registry);
    void swapBuffers(Registry &registry);
    void destroy(Registry &registry);
};