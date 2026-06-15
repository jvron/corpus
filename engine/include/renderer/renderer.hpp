#pragma once

#include "engine/world.hpp"
//#include "renderer/opengl/gl_backend.hpp"

namespace Renderer{

    void init(World &world);
    
    void uploadMesh(World &world);

    void beginFrame(World &world);
    void renderScene(World &world);
    void endFrame(World &world);
};