#include "resources/asset_loader.hpp"
#include "header.hpp"

void loadcubeMesh(World &world, Mesh& mesh) {

    Renderable cubeObj = {.visible = true};

    MeshData cubeData;
   
    cubeData.vertices = {

    // FRONT (Z+)
    {{-0.5f,-0.5f, 0.5f}, {1,0,0,1}, {0,0}, {0,0,1}},
    {{ 0.5f,-0.5f, 0.5f}, {0,1,0,1}, {1,0}, {0,0,1}},
    {{ 0.5f, 0.5f, 0.5f}, {0,0,1,1}, {1,1}, {0,0,1}},
    {{-0.5f, 0.5f, 0.5f}, {1,1,0,1}, {0,1}, {0,0,1}},

    // BACK (Z-)
    {{ 0.5f,-0.5f,-0.5f}, {1,0,1,1}, {0,0}, {0,0,-1}},
    {{-0.5f,-0.5f,-0.5f}, {0,1,1,1}, {1,0}, {0,0,-1}},
    {{-0.5f, 0.5f,-0.5f}, {0.5f,0.5f,0.5f,1}, {1,1}, {0,0,-1}},
    {{ 0.5f, 0.5f,-0.5f}, {1,1,1,1}, {0,1}, {0,0,-1}},

    // LEFT (X-)
    {{-0.5f,-0.5f,-0.5f}, {1,0,0,1}, {0,0}, {-1,0,0}},
    {{-0.5f,-0.5f, 0.5f}, {0,1,0,1}, {1,0}, {-1,0,0}},
    {{-0.5f, 0.5f, 0.5f}, {0,0,1,1}, {1,1}, {-1,0,0}},
    {{-0.5f, 0.5f,-0.5f}, {1,1,0,1}, {0,1}, {-1,0,0}},

    // RIGHT (X+)
    {{ 0.5f,-0.5f, 0.5f}, {1,0,1,1}, {0,0}, {1,0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,1,1,1}, {1,0}, {1,0,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0.5f,0.5f,0.5f,1}, {1,1}, {1,0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {1,1,1,1}, {0,1}, {1,0,0}},

    // TOP (Y+)
    {{-0.5f, 0.5f, 0.5f}, {1,0,0,1}, {0,0}, {0,1,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,1,0,1}, {1,0}, {0,1,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0,0,1,1}, {1,1}, {0,1,0}},
    {{-0.5f, 0.5f,-0.5f}, {1,1,0,1}, {0,1}, {0,1,0}},

    // BOTTOM (Y-)
    {{-0.5f,-0.5f,-0.5f}, {1,0,1,1}, {0,0}, {0,-1,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,1,1,1}, {1,0}, {0,-1,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0.5f,0.5f,0.5f,1}, {1,1}, {0,-1,0}},
    {{-0.5f,-0.5f, 0.5f}, {1,1,1,1}, {0,1}, {0,-1,0}},
};
    cubeData.indices = {
        0, 1, 2,  2, 3, 0,        // front
        4, 5, 6,  6, 7, 4,        // back
        8, 9,10, 10,11, 8,        // left
        12,13,14, 14,15,12,        // right
        16,17,18, 18,19,16,        // top
        20,21,22, 22,23,20         // bottom
    };

        VertexAttribute posAttribute = {
            .location = 0, 
            .componentCount = 3, 
            .type = ComponentType::Float, 
            .normalized = false, 
            .relativeOffset = offsetof(Vertex, position) 
        };

        VertexAttribute colorAttribute = {
            .location = 1, 
            .componentCount = 4, 
            .type = ComponentType::Float, 
            .normalized = false, 
            .relativeOffset = offsetof(Vertex, color)
        };

        VertexAttribute texCoord = {
            .location = 2, 
            .componentCount = 2, 
            .type = ComponentType::Float,  
            .normalized = false, 
            .relativeOffset = offsetof(Vertex, uv)
        };

        VertexAttribute normal = {
            .location = 3, 
            .componentCount = 3, 
            .type = ComponentType::Float, 
            .normalized = false, 
            .relativeOffset = offsetof(Vertex, normal)
        };

    cubeData.vertexLayout.attributes.push_back(posAttribute);
    cubeData.vertexLayout.attributes.push_back(colorAttribute);
    cubeData.vertexLayout.attributes.push_back(texCoord);
    cubeData.vertexLayout.attributes.push_back(normal);

    cubeData.vertexLayout.bindingIndex = 0;
    cubeData.vertexLayout.stride = sizeof(Vertex);

    mesh = world.resourceManager.loadMesh(cubeData, "cube");
}
