#pragma once

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

    size_t dimension() const { return vector.size(); }
};