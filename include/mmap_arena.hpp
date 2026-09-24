#pragma once

#include <cstddef>
#include <string>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

class MmapArena
{
private:
    int m_fd;
    std::size_t m_size;
    char *m_data;

    void init()
    {
        m_fd = -1;
        m_size = 0;
        m_data = nullptr;
    }

public:
    MmapArena()
    {
        init();
    }

    int open_file(const std::string &filename, std::size_t file_size)
    {
        m_fd = open(filename, O_RDWR);
        if (m_fd == -1)
        {
            init();
            return 1;
        }

        m_size = file_size;
        void *mapped = mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);

        if (mapped == MAP_FAILED)
        {
            close(m_fd);
            init();
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
            init();

            return 0;
        }
        else
        {
            // no file open
            return 1;
        }
    }

    int create_file(const std::string &filename, std::size_t file_size)
    {
        m_fd = open(filename, O_RDWR | O_CREAT);
        if (m_fd == -1)
        {
            init();
            return 1;
        }

        m_size = file_size;
        ftruncate(m_fd, m_size);

        void *mapped = mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);

        m_data = static_cast<char *>(mapped);

        return 0;
    }

    int flush()
    {
        if (m_data == nullptr)
        {
            return 1;
        }

        if (msync(m_data, m_size, MS_SYNC) == -1)
        {
            return 1;
        }

        return 0;
    }

    int grow(std::size_t new_size)
    {
        if (munmap(m_data, m_size) == -1)
        {
            return 1;
        }
        m_size = new_size;
        ftruncate(m_fd, m_size);
        m_data = static_cast<char *>(mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
        return 0;
    }

    char *data()
    {
        return m_data;
    }
};