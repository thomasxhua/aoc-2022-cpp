#include <iostream>
#include <fstream>
#include <string>

bool is_clamped(const uint64_t n, const uint64_t min, const uint64_t max)
{
    return n >= min && n <= max;
}

struct Assignment
{
    uint64_t a0, a1;
    uint64_t b0, b1;
    
    Assignment(const std::string& str)
    {
        static auto f = [](const std::string& s)
        {
            size_t pos = s.find_last_of("-");
            uint64_t x = atoi(s.substr(0,pos).c_str());
            uint64_t y = atoi(s.substr(pos+1).c_str());
            return std::pair<uint64_t,uint64_t>(x,y);
        };
        size_t comma_pos = str.find_last_of(",");
        auto a = f(str.substr(0, comma_pos));
        auto b = f(str.substr(comma_pos+1));
        a0 = a.first;
        a1 = a.second;
        b0 = b.first;
        b1 = b.second;
    }
    
    bool is_first_contained()
    {
        return \
            is_clamped(a0, b0, b1) || is_clamped(a1, b0, b1)
            ||
            is_clamped(b0, a0, a1) || is_clamped(b1, a0, a1);
    }
};

int main()
{
    std::fstream file("input");
    std::string line;
    uint64_t count = 0;
    while (std::getline(file, line))
    {
        Assignment ass(line);
        if (ass.is_first_contained())
            ++count;
    }
    std::cout << count;
    return 0;
}

