#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <string_view>
#include <unordered_map>

using StringID = uint32_t;

class StringPool {

public:
    StringID intern(std::string_view string);
    std::string_view getString(StringID id) const;

    size_t size() const;

private:
    std::unordered_map<std::string, StringID> lookup;
    std::vector<std::string> strings;
};