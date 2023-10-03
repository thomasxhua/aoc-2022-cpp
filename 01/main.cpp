#include <iostream>
#include <fstream>
#include <vector>

struct triple
{
    uint64_t a,b,c;
    void push(uint64_t n)
    {
        if (n > a)
        {
            c = b; b = a; a = n;
        }
        else if (n > b)
        {
            c = b; b = n;
        }
        else if (n > c)
        {
            c = b;
        }
    }
    uint64_t sum()
    {
        return a+b+c;
    }
};

int main()
{
    std::fstream file("input");
    triple current_maxs     = {0,0,0};
    uint64_t current_num    = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            current_maxs.push(current_num);
            current_num = 0;
        }
        else
        {
            current_num += atoi(line.c_str());
        }
    }
    std::cout << current_maxs.sum();
    return 0;
}
