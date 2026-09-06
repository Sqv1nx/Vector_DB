#include "data_structure.hpp"

#include <queue>
#include <vector>
#include <cstddef>
#include <algorithm>

template <typename T>
float squared_l2_distance(const std::vector<T> &a, const std::vector<T> &b)
{
    if (a.size() != b.size())
        return -1;

    float dist{0.0f};

    for (std::size_t i{0}; i < a.size(); ++i)
    {
        float diff{a[i] - b[i]};
        dist += diff * diff;
    }

    return dist;
}

template <typename T>
std::vector<int> flat_search(std::size_t k, const VectorRecord<T> &query, const std::vector<VectorRecord<T>> &records)
{
    std::priority_queue<std::pair<float, int>> top_k;

    for (std::size_t i{0}; i < records.size(); ++i)
    {
        float distance{squared_l2_distance(records[i].vector, query.vector)};

        if (top_k.size() < k)
            top_k.push({distance, records[i].id});
        else if (top_k.top().first > distance)
        {
            top_k.pop();
            top_k.push({distance, records[i].id});
        }
    }

    // extracting closest-k elements and reversing because top_k is max-heap
    std::vector<int> closest_k_id;
    while (!top_k.empty())
    {
        closest_k_id.push_back(top_k.top().second);
        top_k.pop();
    }

    std::reverse(closest_k_id.begin(), closest_k_id.end());

    return closest_k_id;
}
// Explicit instantiation for float
template std::vector<int> flat_search<float>(std::size_t, const VectorRecord<float>&, const std::vector<VectorRecord<float>>&);
