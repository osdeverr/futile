#pragma once

#include <cstdio>
#include <filesystem>

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

        void close();

        File &operator=(File &&other)
        {
            if (mHandle)
                fclose_unsafe();

            mHandle = other.mHandle;
            other.mHandle = nullptr;

            return *this;
        }

        template <class StringT = ulib::string>
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

        template <class StringT, class EncodingT = ulib::argument_encoding_or_die_t<StringT>>
        void write(const StringT &str)
        {
            ulib::EncodedStringView<EncodingT> view = str;

            size_t ss = write(view.data(), view.size_in_bytes());
            if (ss == -1)
                throw std::runtime_error("Failed to write file");
        }

        void write(ulib::buffer_view view)
        {
            size_t ss = write(view.data(), view.size_in_bytes());
            if (ss == -1)
                throw std::runtime_error("Failed to write file");
        }

        template <class StringT = ulib::string, class ContainerT = ulib::List<StringT>,
                  class EncodingT = ulib::string_encoding_t<StringT>,
                  std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
        ContainerT lines()
        {
            auto lineBreak = ulib::Convert<EncodingT>(u8"\n");
            auto contents = read<StringT>();

            auto split = ulib::split(contents, lineBreak);
            return ContainerT{split.begin(), split.end()};
        }

        void flush();

    private:
        void fclose_unsafe();

        FILE *mHandle;
    };

    File open(const fs::path &path, const char *rr = kDefaultFileOpenType);

} // namespace futile