#include <vector>
#include <string>

#include "corrector.h"

int main()
{
    // set up
    Corrector c;
    c.train("../big.txt");

    std::string input;
    while (true){
        getline(std::cin, input);
        std::cout << input << "-->" << c.correct(input) << std::endl;
    }
}
