# Vector DB - Brute-Force kNN Search

A high-performance foundational vector search engine implemented in modern C++20. 

This repository currently implements **Phase 0** of a custom Vector Database, focusing on data ingestion from standard binary benchmark formats and establishing an exact-match baseline using Brute-Force k-Nearest Neighbors (kNN).

## 🚀 Accomplishments So Far

*   **Modern C++ Architecture:** Established a robust `CMake` build system leveraging standard C++20 features.
*   **Flexible Data Structures:** Created templated core structures (`VectorRecord<T>` and `Metadata`) that smoothly handle dynamic, high-dimensional vector data (like `float`) and integer ground-truth IDs, along with arbitrary payload metadata attachments.
*   **Binary Dataset Loader:** Implemented a fast and strict binary file parser (`VectorStoreIO`) specifically designed to read `.fvecs` and `.ivecs` formats used by standard ANN (Approximate Nearest Neighbor) benchmarks like SIFT.
*   **Exact kNN Baseline (`flat_search`):** 
    *   Developed a mathematically exact linear scan search using a **Squared L2 (Euclidean) distance** metric.
    *   Utilizes a max-heap priority queue (`std::priority_queue`) to efficiently maintain the top-$k$ nearest neighbors during the scan without fully sorting the dataset.
*   **Benchmark Verification:** The system has been fully validated against the `SIFT-small` benchmark dataset (10,000 base vectors, 100 query vectors). The exact brute-force search successfully computes a **perfect 100% Recall@100** when compared against the provided ground truth dataset.

## 🛠️ Building and Running

This project uses an out-of-source CMake build approach.

```bash
# 1. Create a dedicated build directory
mkdir build && cd build

# 2. Configure the project with CMake
cmake ..

# 3. Compile the executable
make

# 4. Run the evaluation
./Vector_DB
```

## 🧹 How to Clean CMake

Because we use a dedicated `build/` directory, completely cleaning your CMake environment is incredibly simple and safe. If you ever run into caching issues or just want to wipe the slate clean, simply delete the build folder and start over:

```bash
# From the root of your project:
rm -rf build/
mkdir build
```
This safely destroys all generated `Makefiles`, `CMakeCache.txt` files, and compiled object files without leaving a mess in your source code!
