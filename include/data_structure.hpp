#pragma once

#include <cstddef>
#include <string>
#include <any>
#include <unordered_map>
#include <vector>

struct Metadata
{
    std::unordered_map<std::string, std::any> meta;
};

template <typename T>
struct VectorRecord
{
    int id{};
    std::vector<T> vector;
    Metadata metadata;

    std::size_t dimension() const { return vector.size(); }
};