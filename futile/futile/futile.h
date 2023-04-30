#pragma once
#include <cstdio>
#include <filesystem>
#include <vector>

#include <string_view>

#include <ulib/encodings/convert.h>
#include <ulib/split.h>
#include <ulib/string.h>

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

        size_t read(void *buf, size_t size);
        size_t write(const void *buf, size_t size);
        size_t size();

        template <class StringT = std::string>
        StringT read()
        {
            using CharT = typename StringT::value_type;
            size_t fileSize = size();

            if constexpr (sizeof(CharT) > 1)
            {
                volatile size_t ex = (fileSize / sizeof(CharT));
                if (ex * sizeof(CharT) != ex)
                    throw std::runtime_error("Invalid file/char");
            }

            StringT str;
            str.resize(fileSize / sizeof(CharT));

            if (read((void *)str.data(), fileSize) == -1)
                throw std::runtime_error("Failed to read file");

            return str;
        }

        template <class StringT, class CharT = typename StringT::value_type>
        void write(const StringT &str)
        {
            size_t ss = write((char *)str.data(), str.size() * sizeof(char));
            if (ss == -1)
                throw std::runtime_error("Failed to write file");
        }

        template <class StringT = std::string, class ContainerT = std::vector<StringT>>
        ContainerT lines()
        {
            using value_type = typename StringT::value_type;

            std::basic_string<value_type> contents = read<StringT>();
            std::basic_string<value_type> line_break = ulib::Convert<ulib::LiteralEncodingT<value_type>>(u8"\n");

            auto split = ulib::split(contents, line_break);
            return ContainerT{split.begin(), split.end()};
        }

    private:
        FILE *mHandle;
    };

    File open(const fs::path &path, const char *rr = kDefaultFileOpenType);

} // namespace futile