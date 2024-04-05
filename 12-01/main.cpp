#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define PRINT(x) std::cout << x
#define DELOG(x) PRINT("(DELOG) " << x << std::endl)

using Map = std::vector<std::string>;

std::ostream& operator<<(std::ostream& target, const Map& map)
{
    std::for_each(map.begin(), map.end(), [&target](const std::string& str)
    {
        target << str << "\n";
    });
    return target;
};

using Position = std::pair<uint64_t,uint64_t>;

std::ostream& operator<<(std::ostream& target, const Position& pos)
{
    target << "(" << pos.first << "," << pos.second << ")";
    return target;
};

Position find_in_map(const Map& map, const char target)
{
    uint64_t x      = 0;
    Position pos    = {-1,-1};
    std::for_each(map.begin(), map.end(), [&](const std::string& str)
    {
        uint64_t y = 0;
        std::for_each(str.begin(), str.end(), [&](const char c)
        {
            if (c == target)
            {
                pos = {x,y};
            }
            ++y;
        });
        ++x;
    });
    return pos;
}

using Path = std::vector<Position>;

std::ostream& operator<<(std::ostream& target, const Path& path)
{
    target << "{" << " ";
    std::for_each(path.begin(), path.end(), [&target](const Position& pos)
    {
        target << pos << " ";
    });
    target << "}";
    return target;
};

Path shortest_path(const Map& map, const Position& start, const Position& end)
{
    return {};
}

int main()
{
    std::fstream file("input");
    std::string line;
    Map map;
    while (std::getline(file, line))
    {
        map.push_back(line);
    }
    PRINT(map);
    const Position start    = find_in_map(map, 'S');
    const Position end      = find_in_map(map, 'E');
    DELOG("start pos:   " << start);
    DELOG("end pos:     " << end);
    const Path path         = shortest_path(map, start, end);
    DELOG("path:        " << path);
    DELOG("path count:  " << path.size());
    return 0;
}

