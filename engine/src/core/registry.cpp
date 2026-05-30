#include "engine/registry.hpp"
#include <vector>

void Registry::addComponent(Component Component) {
    components.push_back(Component);
}