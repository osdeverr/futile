#include <futile/futile.h>
#include <gtest/gtest.h>


/*

#include <iostream>

#include <futile/futile.h>
#include <ulib/format.h>

int main()
{
    try
    {
        for (auto &line : futile::open("test.txt").lines())
        {
            fmt::print("Line: {}\n", line);
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}


*/

TEST(Futile, CreateFileAndOpen)
{
    EXPECT_NO_THROW({
        auto file = futile::open("test_file.txt", "w");
        file = futile::open("test_file.txt", "r");
    });
}

TEST(Futile, WriteAndRead)
{
    auto file = futile::open("test_file.txt", "w");
    file.write("hello");

    file = futile::open("test_file.txt", "r");
    auto text = file.read();

    ASSERT_EQ(text, "hello");
}

TEST(Futile, Raw)
{
    auto file = futile::open("test_file.bin", "wb");

    ulib::List<int> values = {1, 2, 3, 4, 5};
    file.write(values.raw());

    file = futile::open("test_file.bin", "rb");

    ulib::List<int> result(5);
    size_t readen = file.read(result.data(), result.size_in_bytes());

    ASSERT_EQ(readen, values.size_in_bytes());
    ASSERT_EQ(result, values);
}