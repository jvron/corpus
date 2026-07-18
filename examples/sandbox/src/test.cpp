#include "ecs/components.hpp"
#include "ecs/view.hpp"
#include "engine/systems.hpp"
#include "engine/world.hpp"
#include "platform/input.hpp"
#include "platform/window.hpp"

#include <GLFW/glfw3.h>


enum class Direction {
    Up,
    Down,
    Left,
    Right
};


void moveModel(World& world, Direction direction)
{
    for (auto [entity, model, transform] : View<Model, Transform>(world.registry))
    {
        switch (direction)
        {
            case Direction::Up:
                transform.position.y += 0.1f;
                break;

            case Direction::Down:
                transform.position.y -= 0.1f;
                break;

            case Direction::Left:
                transform.position.x -= 0.1f;
                break;

            case Direction::Right:
                transform.position.x += 0.1f;
                break;
        }
    }
}


void testCamera(World& world)
{
    Camera& camera =
        world.registry.getComponent<Camera>(world.activeCamera);

    CameraController& controller =
        world.registry.getComponent<CameraController>(world.activeCamera);

    float deltaTime = world.engineState.time.deltaTime;


    if (Input::isKeyDown(world, Key::A))
        CameraControlSystem::moveLeft(camera, controller, deltaTime);

    if (Input::isKeyDown(world, Key::D))
        CameraControlSystem::moveRight(camera, controller, deltaTime);

    if (Input::isKeyDown(world, Key::W))
        CameraControlSystem::moveForward(camera, controller, deltaTime);

    if (Input::isKeyDown(world, Key::S))
        CameraControlSystem::moveBackwards(camera, controller, deltaTime);


    if (Input::isKeyDown(world, Key::Escape))
        Window::close(world);


    if (Input::isKeyDown(world, Key::UpArrow))
        moveModel(world, Direction::Up);

    if (Input::isKeyDown(world, Key::DownArrow))
        moveModel(world, Direction::Down);

    if (Input::isKeyDown(world, Key::LeftArrow))
        moveModel(world, Direction::Left);

    if (Input::isKeyDown(world, Key::RightArrow))
        moveModel(world, Direction::Right);


    if (Input::isKeyPressed(world, Key::LeftControl))
    {
        bool cursorEnabled =
            world.engineState.inputState.cursorEnabled;

        if (cursorEnabled)
        {
            Input::disableCursor(world);

            controller.enableMouseLook = true;
            controller.enableMovement = true;
        }
        else
        {
            Input::enableCursor(world);

            controller.enableMouseLook = false;
            controller.enableMovement = false;
        }
    }
}


void testUniform(World& world)
{
    float time = glfwGetTime();

    for (auto [entity, light] : View<PointLight>(world.registry))
    {
        float r = sin(time) * 0.5f + 0.5f;
        float g = sin(time + 2.0f) * 0.5f + 0.5f;
        float b = sin(time + 4.0f) * 0.5f + 0.5f;

        (void)r;
        (void)g;
        (void)b;

        // Example animated light color
        // light.color = {r, g, b, 1.0f};
    }
}


void testTransform(World& world)
{
    Camera& camera =
        world.registry.getComponent<Camera>(world.activeCamera);

    // update spotlight position
    for (auto [entity, light] : View<Spotlight>(world.registry))
    {
        light.position = camera.position;
        light.direction = camera.front;
    }
}