#pragma once
#include "common.h"
#include <cstdlib>

namespace file
{

struct FileData
{
    noncopyable(FileData);

    void* m_contents;
    size_t m_size;

    FileData() = default;
    FileData(FileData&& other)
    {
        m_contents = other.m_contents;
        m_size = other.m_size;
        other.m_contents = nullptr;
        other.m_size = 0;
    }

    ~FileData()
    {
        if (m_contents)
        {
            free(m_contents);
        }
    }
};

common::Result<FileData> readAll(char* filename);
bool writeAll(char* filename, void* contents, uint size);

}