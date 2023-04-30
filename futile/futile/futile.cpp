#include "futile.h"

#include <ulib/format.h>
#include <ulib/wchar.h>

namespace futile
{

    FILE *OpenFileF(const fs::path &path, const char *rr)
    {
#ifdef _WIN32
        return _wfopen((wchar_t *)path.u16string().c_str(), ulib::wstr((char8 *)rr).c_str());
#else
        return fopen(path.c_str(), rr);
#endif
    }

    size_t FileSize(FILE *handle)
    {
        assert(handle);

        FILE *pFile = (FILE *)handle;

        long fcurr = ftell(pFile);
        fseek(pFile, 0, SEEK_END);
        long fsize = ftell(pFile);
        fseek(pFile, fcurr, SEEK_SET);

        return size_t(fsize);
    }

    File::File(const fs::path &path, const char *rr)
    {
        mHandle = OpenFileF(path, rr);
        if (!mHandle)
            throw std::runtime_error(ulib::format("Failed open file {}", path));
    }

    File::~File()
    {
        if (mHandle)
            ::fclose(mHandle);
    }

    size_t File::Read(void *buf, size_t size) { return ::fread(buf, 1, size, mHandle); }
    size_t File::Write(const void *buf, size_t size) { return ::fwrite(buf, 1, size, mHandle); }
    size_t File::Size() { return FileSize(mHandle); }

    size_t File::read(void *buf, size_t size) { return Read(buf, size); }
    size_t File::write(const void *buf, size_t size) { return Write(buf, size); }
    size_t File::size() { return Size(); }

    File Open(const fs::path &path, const char *rr)
    {
        File ff(path, rr);
        return ff;
    }

    File open(const fs::path &path, const char *rr) { return Open(path, rr); }

} // namespace futile