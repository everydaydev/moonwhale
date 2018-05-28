#ifdef _WIN32
#include "file.h"
#include "win32.h"
#include "logger.h"

namespace file
{

common::Result<FileData> readAll(char* filename)
{
    common::Result<FileData> result = {};
    auto& fileData = result.value;

    HANDLE fileHandle =
        CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, 0);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        l_error("Failed to open %s", filename);
        return result;
    }

    LARGE_INTEGER fileSize;
    if (GetFileSizeEx(fileHandle, &fileSize))
    {
        fileData.m_contents = malloc(fileSize.QuadPart);

        if (fileData.m_contents)
        {
            uint32_t fileSize32 = static_cast<uint32_t>(fileSize.QuadPart);
            DWORD bytesRead;
            if (ReadFile(fileHandle, fileData.m_contents, fileSize32, &bytesRead, nullptr) &&
                fileSize32 == bytesRead)
            {
                fileData.m_size = fileSize32;
            }
            else
            {
                l_error("Bytes read is not same as file size of %s", filename);
                free(fileData.m_contents);
                fileData.m_contents = nullptr;
            }
        }
        else
        {
            l_error("Failed to allocate memory");
        }
    }
    else
    {
        l_error("Failed to get file size of %s", filename);
    }

    CloseHandle(fileHandle);

    return result;
}

bool writeAll(char* filename, void* contents, uint size)
{
    HANDLE fileHandle =
        CreateFileA(filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, 0);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        l_error("Failed to open %s", filename);
        return false;
    }

    DWORD bytesWritten;
    bool result = false;
    if (WriteFile(fileHandle, contents, size, &bytesWritten, nullptr))
    {
        result = (bytesWritten == size);
    }
    CloseHandle(fileHandle);

    return result;
}

#endif//_WIN32

}