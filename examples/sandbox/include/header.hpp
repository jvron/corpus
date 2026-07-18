#pragma once 

#include "engine/world.hpp"

void checkInput(World& world);
void readPos(World& world);
void testCamera(World& world);
void testUniform(World &world);
void testTransform(World &world);

void loadcubeMesh(World &world, Mesh& mesh);
void loadShader(World& world, MaterialAsset& material);
void loadLightShader(World& world, MaterialAsset& material);
void loadTexture(World& world, MaterialAsset& material);
