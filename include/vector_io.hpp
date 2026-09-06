#pragma once

#include "data_structure.hpp"

#include <cstdint>
#include <fstream>
#include <vector>
#include <stdexcept>

class VectorStoreIO
{
public:
    template <typename T>
    static std::vector<VectorRecord<T>> read_vecs(const std::string &file_path)
    {
        std::ifstream input_file(file_path, std::ios::binary);
        if (!input_file)
            throw std::runtime_error("File could not be opened.");

        std::vector<VectorRecord<T>> records;
        std::int32_t dim{};
        int current_id{0};

        while (input_file.read(reinterpret_cast<char *>(&dim), sizeof(dim)))
        {
            VectorRecord<T> new_record;
            new_record.vector.resize(dim);
            new_record.id = current_id;
            current_id++;

            input_file.read(reinterpret_cast<char *>(new_record.vector.data()), dim * sizeof(T));

            records.push_back(new_record);
        }

        return records;
    }
};