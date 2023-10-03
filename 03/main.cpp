#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

uint64_t char_to_priority(const char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    return c - 'A' + 1 + 26;
}

int main()
{
    std::fstream file("input");
    std::string line;
    std::vector<std::string> lines;
    uint64_t sum = 0;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    uint64_t i = 0;
    while (i < lines.size())
    {
        std::map<char, uint64_t> m;
        for (uint64_t j=0; j<3; ++j, ++i)
        {
            const auto& str = lines[i];
            std::set<char> s;
            for (const char c : str)
            {
                s.insert(c);
            }
            for (const char c : s)
            {
                auto it = m.find(c);
                if (it != m.end())
                    m[c] += 1;
                else
                    m[c] = 1;
            }
        }
        for (const auto& pair : m)
        {
            if (pair.second == 3)
            {
                sum += char_to_priority(pair.first);
            }
        }
    }
    std::cout << sum;
    return 0;
}

#if 0
    while (std::getline(file, line))
    {
        for (uint64_t i = (line.length() >> 1); i < line.length(); ++i)
        {
            const char c = line[i];
            if (is_in_substr_firsthalf(c, line))
            {
                sum += char_to_priority(c);
                break;
            }
        }
    }
#endif
