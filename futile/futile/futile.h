#pragma once

#include <cstdio>
#include <filesystem>

namespace futile
{
    namespace fs = std::filesystem;

    constexpr auto kDefaultFileOpenType = "r";

    class File
    {
    public:
        File() : mHandle(nullptr) {}
        File(const fs::path &path, const char *rr = kDefaultFileOpenType);
        File(const File &other) = delete;
        File(File &&other) : mHandle(other.mHandle) { other.mHandle = nullptr; }

        ~File();

        size_t Read(void *buf, size_t size);
        size_t Write(const void *buf, size_t size);
        size_t Size();

        template <class StringT>
        StringT Read()
        {
            using CharT = typename StringT::value_type;
            size_t fileSize = Size();

            if constexpr (sizeof(CharT) > 1)
            {
                volatile size_t ex = (fileSize / sizeof(CharT));
                if (ex * sizeof(CharT) != ex)
                    throw std::runtime_error("Invalid file/char");
            }

            StringT str;
            str.resize(fileSize);

            if (Read((void *)str.data(), fileSize) == -1)
                throw std::runtime_error("Failed to read file");

            return str;
        }

        inline size_t read(void *buf, size_t size) { return Read(buf, size); }
        inline size_t write(const void *buf, size_t size) { return Write(buf, size); }
        inline size_t size() { return Size(); }

        template <class StringT>
        StringT read()
        {
            return Read<StringT>();
        }

    private:
        FILE *mHandle;
    };

    File Open(const fs::path &path, const char *rr = kDefaultFileOpenType);
    inline File open(const fs::path &path, const char *rr = kDefaultFileOpenType) { return Open(path, rr); }

} // namespace futile