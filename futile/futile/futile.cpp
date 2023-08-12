#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "futile.h"

#include <ulib/fmt/path.h>
#include <ulib/format.h>
#include <ulib/string.h>

#include <stdio.h>

namespace futile
{
    FILE *OpenFileF(const fs::path &path, const char *rr)
    {
#ifdef _WIN32
        return _wfopen(path.c_str(), ulib::wstr((char8 *)rr).c_str());
#else
        return fopen(path.c_str(), rr);
#endif
    }

    size_t FileSize(FILE *handle)
    {
        assert(handle);

        FILE *pFile = (FILE *)handle;

        long fcurr = std::ftell(pFile);
        std::fseek(pFile, 0, SEEK_END);
        long fsize = std::ftell(pFile);
        std::fseek(pFile, fcurr, SEEK_SET);

        return size_t(fsize);
    }

    File::File(const fs::path &path, const char *rr)
    {
        mHandle = OpenFileF(path, rr);
        if (!mHandle)
            throw std::runtime_error(ulib::format("Failed to open file {}", path));
    }

    File::~File()
    {
        if (mHandle)
            std::fclose(mHandle);
    }

    void File::close()
    {
        if (mHandle)
        {
            std::fclose(mHandle);
            mHandle = nullptr;
        }
    }

    size_t File::read(void *buf, size_t size) { return std::fread(buf, 1, size, mHandle); }
    size_t File::write(const void *buf, size_t size) { return std::fwrite(buf, 1, size, mHandle); }
    size_t File::size() { return FileSize(mHandle); }

    void File::fclose_unsafe() { std::fclose(mHandle); }

    File open(const fs::path &path, const char *rr)
    {
        File ff(path, rr);
        return ff;
    }

    void File::flush() { std::fflush(mHandle); }

} // namespace futile