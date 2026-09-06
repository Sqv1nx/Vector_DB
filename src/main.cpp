#include "data_structure.hpp"
#include "vector_io.hpp"
#include "flat_search.hpp"

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>

int main()
{
    std::cout << "Loading SIFT-small base dataset..." << std::endl;
    auto base_vectors = VectorStoreIO::read_vecs<float>("../tests/siftsmall_base.fvecs");

    // Validation Checkpoint (Section 2)
    assert(base_vectors.size() == 10000 && "Base dataset size should be exactly 10,000");
    if (!base_vectors.empty())
    {
        assert(base_vectors[0].dimension() == 128 && "Base vectors should have a dimension of 128");
    }
    std::cout << "Validation Checkpoint Passed: Loaded 10,000 vectors of dimension 128." << std::endl;

    std::cout << "Loading SIFT-small query dataset..." << std::endl;
    auto query_vectors = VectorStoreIO::read_vecs<float>("../tests/siftsmall_query.fvecs");

    std::cout << "Loading SIFT-small ground truth dataset..." << std::endl;
    auto ground_truth = VectorStoreIO::read_vecs<int32_t>("../tests/siftsmall_groundtruth.ivecs");

    assert(query_vectors.size() == 100 && "Query dataset should have exactly 100 queries");
    assert(ground_truth.size() == 100 && "Ground truth dataset should have exactly 100 entries");

    // Recall Validation (Section 3)
    std::cout << "Starting Recall Validation..." << std::endl;

    int k = 100;
    double total_recall = 0.0;

    for (size_t i = 0; i < query_vectors.size(); ++i)
    {
        // Get the top-k results from our brute-force search
        std::vector<int> results = flat_search<float>(k, query_vectors[i], base_vectors);

        // Get the ground truth results for this query
        const std::vector<int32_t>& gt_indices = ground_truth[i].vector;
        
        // Calculate the intersection size
        int intersection = 0;
        std::unordered_set<int32_t> gt_set(gt_indices.begin(), gt_indices.end());
        for (int id : results)
        {
            if (gt_set.find(id) != gt_set.end())
            {
                intersection++;
            }
        }

        double recall = static_cast<double>(intersection) / k;
        total_recall += recall;
    }

    double average_recall = total_recall / query_vectors.size();
    
    std::cout << "Average Recall@100: " << (average_recall * 100) << "%" << std::endl;
    
    if (average_recall == 1.0)
    {
        std::cout << "SUCCESS: Brute-Force kNN is perfectly accurate." << std::endl;
    }
    else
    {
        std::cout << "FAILURE: Brute-Force kNN is not fully accurate." << std::endl;
    }

    return 0;
}
