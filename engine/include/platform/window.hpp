#pragma once

struct World;

namespace Window {

    void create(World &world);

    void pollEvents(World &world);
    void swapBuffers(World &world);
    void destroy(World &world);

    bool shouldClose(World &world);
    void close(World &world);
};