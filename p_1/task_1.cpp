#include <iostream>
#include <iomanip>

int main() {

    double num;

    std::cout << "Enter fractional number pls -> ";
    std::cin >> num;

    if (std::cin.fail()) {
        std::cout << "Invalid input!" << std::endl;

    } else {
        std::cout << "Result is "
                  << std::fixed << std::setprecision(2)
                  << num << std::endl;
    }

    return 0;
}
