#pragma once
#include "data_structure.hpp"
#include <vector>
#include <cstddef>

template <typename T>
std::vector<int> flat_search(std::size_t k, const VectorRecord<T> &query, const std::vector<VectorRecord<T>> &records);
