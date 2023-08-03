#include <iostream>

#include <futile/futile.h>

int main()
{
    try
    {
        for (auto &line : futile::open("test.txt").lines())
        {
            std::cout << "Line: " << line << "\n";
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
