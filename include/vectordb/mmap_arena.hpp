#pragma once

#include <cstddef>
#include <string>
#include <utility>

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

    void teardown()
    {
        if (m_fd != -1)
        {
            if (m_data != nullptr)
            {
                munmap(m_data, m_size);
            }
            close(m_fd);
        }
        init();
    }

public:
    MmapArena()
    {
        init();
    }

    MmapArena(const MmapArena &) = delete;
    MmapArena &operator=(const MmapArena &) = delete;

    MmapArena(MmapArena &&other) noexcept
        : m_fd(other.m_fd), m_size(other.m_size), m_data(other.m_data)
    {
        other.init();
    }

    MmapArena &operator=(MmapArena &&other) noexcept
    {
        if (this != &other)
        {
            teardown();
            m_fd = other.m_fd;
            m_size = other.m_size;
            m_data = other.m_data;
            other.init();
        }
        return *this;
    }

    ~MmapArena()
    {
        teardown();
    }

    int open_file(const std::string &filename)
    {
        teardown();

        m_fd = open(filename.c_str(), O_RDWR);
        if (m_fd == -1)
        {
            init();
            return 1;
        }

        struct stat st{};
        if (fstat(m_fd, &st) == -1)
        {
            close(m_fd);
            init();
            return 1;
        }
        m_size = static_cast<std::size_t>(st.st_size);

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

    int create_file(const std::string &filename, std::size_t file_size)
    {
        teardown();

        m_fd = open(filename.c_str(), O_RDWR | O_CREAT, 0644);
        if (m_fd == -1)
        {
            init();
            return 1;
        }

        if (ftruncate(m_fd, static_cast<off_t>(file_size)) == -1)
        {
            close(m_fd);
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
        if (m_fd == -1)
        {
            return 1;
        }

        if (munmap(m_data, m_size) == -1)
        {
            return 1;
        }

        if (close(m_fd) == -1)
        {
            init();
            return 1;
        }

        init();
        return 0;
    }

    int flush()
    {
        if (m_fd == -1)
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
        if (m_fd == -1)
        {
            return 1;
        }

        if (new_size <= m_size)
        {
            return 1;
        }

        if (munmap(m_data, m_size) == -1)
        {
            return 1;
        }
        m_data = nullptr;

        if (ftruncate(m_fd, static_cast<off_t>(new_size)) == -1)
        {
            teardown();
            return 1;
        }

        void *mapped = mmap(nullptr, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
        if (mapped == MAP_FAILED)
        {
            teardown();
            return 1;
        }

        m_data = static_cast<char *>(mapped);
        m_size = new_size;

        return 0;
    }

    char *data() { return m_data; }
    const char *data() const { return m_data; }
    std::size_t size() const { return m_size; }
};