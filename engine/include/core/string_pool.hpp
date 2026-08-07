#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct StringID {
    uint32_t value {};
};

class StringPool {

public:
    StringID intern(std::string_view string);
    std::string_view getString(StringID id) const;

    size_t size() const;

private:
    std::unordered_map<std::string, StringID> lookup;
    std::vector<std::string> strings;
};