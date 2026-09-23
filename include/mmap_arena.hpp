#pragma once

#include <cstddef>
#include <string>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

class MmapArena
{
private:
    int m_fd;
    std::size_t m_size;
    char *m_data;

public:
    MmapArena()
        : m_fd(-1), m_size(0), m_data(nullptr)
    {
    }

    int open_file(const std::string &filename, std::size_t file_size)
    {
        m_fd = open(filename, O_RDWR | O_CREAT, 0644);
        if (m_fd == -1)
        {
            // error handling
            return 1;
        }

        m_size = file_size;
        void *mapped = mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, m_fd, 0);

        if (mapped == MAP_FAILED)
        {
            // error handling
            close(m_fd);
            m_fd = -1;
            m_size = 0;
            return 1;
        }

        m_data = static_cast<char *>(mapped);

        return 0;
    }

    int close_file()
    {
        if (m_fd != -1)
        {
            if (munmap(m_data, m_size) == -1)
            {
                // error handling
                return 1;
            }

            close(m_fd);

            m_fd = -1;
            m_size = 0;
            m_data = nullptr;

            return 0;
        }
    }
};