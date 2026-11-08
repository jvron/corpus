#include <cassert>

#include "corpus/string_pool.hpp"

StringID StringPool::intern(std::string_view string) {

    auto it = lookup.find(std::string(string));

    if (it != lookup.end()) {
        return it->second;
    }

    size_t index = strings.size();

    StringID id = static_cast<uint32_t>(index);

    strings.emplace_back(string);

    lookup[std::string(string)] = id;

    return id;
}

std::string_view StringPool::getString(StringID id) const {

    assert(id < strings.size() && "ERROR: Invalid StringID provided");

    return strings[id];
}

size_t StringPool::size() const {
    return strings.size();
}