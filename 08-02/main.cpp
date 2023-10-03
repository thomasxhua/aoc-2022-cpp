#include <iostream>
#include <fstream>
#include <string>

#include <vector>

// ~ 35 min

uint64_t maximizer(std::vector<std::string>& trees, const uint64_t I, const uint64_t J)
{
    const uint64_t width    = trees[0].size();
    const uint64_t height   = trees.size();
    const char tree = trees[I][J];
    uint64_t current_max = 0;
    uint64_t top = 0;
    for (uint64_t i=I-1; i!=-1; --i)
        if (tree <= trees[i][J])
        {
            top = i;
            break;
        }
    uint64_t bottom = height-1;
    for (uint64_t i=I+1; i<height; ++i)
        if (tree <= trees[i][J])
        {
            bottom = i;
            break;
        }
    uint64_t left = 0;
    for (uint64_t j=J-1; j!=-1; --j)
        if (tree <= trees[I][j])
        {
            left = j;
            break;
        }
    uint64_t right = width-1;
    for (uint64_t j=J+1; j<width; ++j)
        if (tree <= trees[I][j])
        {
            right = j;
            break;
        }
    return \
        (I - top)
        * (bottom - I)
        * (J - left)
        * (right - J);
}

int main()
{
    std::fstream file("input");
    std::string line;
    std::vector<std::string> trees;
    while (std::getline(file, line))
    {
        trees.push_back(line);
    }
    const uint64_t width    = trees[0].size();
    const uint64_t height   = trees.size();
    uint64_t max = 0;
    for (uint64_t i=0; i<height; ++i)
    {
        for (uint64_t j=0; j<width; ++j)
        {
            const uint64_t current = maximizer(trees, i, j);
            max = (current > max) ? current : max;
        }
    }
    std::cout << max << "\n";
    return 0;
}

