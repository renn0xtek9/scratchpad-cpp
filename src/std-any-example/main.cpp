#include <iostream>
#include <any>

int main()
{
    std::any a;
    a = 12;
    std::cout << std::any_cast<int>(a) << std::endl;
    a = 13.0F;
    std::cout << std::any_cast<float>(a) << std::endl;

    // std::cout << std::any_cast<double>(a) << std::endl; //would throw bad_any_cast

    std::cout << "end" << std::endl;
    return 0;
}
