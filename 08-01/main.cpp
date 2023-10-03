#include <iostream>
#include <fstream>
#include <string>

#include <vector>

bool is_tree_visible(const std::vector<std::string>& trees, const uint64_t I, const uint64_t J)
{
    const uint64_t width    = trees[0].size();
    const uint64_t height   = trees.size();
    const char tree = trees[I][J];
    bool is_visible_top = true;
    for (uint64_t i=0; i<I; ++i)
        if (tree <= trees[i][J])
            is_visible_top = false;
    bool is_visible_bottom = true;
    for (uint64_t i=I+1; i<height; ++i)
        if (tree <= trees[i][J])
            is_visible_bottom = false;
    bool is_visible_left = true;
    for (uint64_t j=0; j<J; ++j)
        if (tree <= trees[I][j])
            is_visible_left = false;
    bool is_visible_right = true;
    for (uint64_t j=J+1; j<width; ++j)
        if (tree <= trees[I][j])
            is_visible_right = false;
    return \
        is_visible_top
        || is_visible_bottom
        || is_visible_left
        || is_visible_right;
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
    uint64_t tree_count = width * 2 + (height - 2) * 2;
    for (uint64_t i=1; i<height-1; ++i)
        for (uint64_t j=1; j<width-1; ++j)
            tree_count += is_tree_visible(trees, i, j);
    std::cout << tree_count << "\n";
    return 0;
}

