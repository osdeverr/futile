#include <iostream>

#include <futile/futile.h>

int main()
{
    try
    {
        auto ky = futile::open("test.txt").read<std::string>();
        futile::open("ky.txt", "w").write(ky);

        std::cout << ky << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
