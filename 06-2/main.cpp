#include <iostream>
#include <fstream>
#include <string>

#include "../tdev.h"

bool is_distinct_str_n2(const std::string& str)
{
    for (int i=0; i<str.size(); ++i)
        for (int j=i+1; j<str.size(); ++j)
            if (str[i] == str[j])
                return false;
    return true;
}

bool is_distinct_str(const std::string& str)
{
    static constexpr size_t ALPH_SIZE = 26;
    bool* exists = new bool[ALPH_SIZE];
    for (uint64_t i=0; i<ALPH_SIZE; ++i)
        exists[ALPH_SIZE] = 0;
    for (const auto& c : str)
    {
        const uint64_t idx  = c - 'a';
        if (exists[idx])
            return false;
        else
            exists[idx] = true;
    }
    delete exists;
    return true;
}

int main()
{
    std::fstream file("input");
    std::string line;
    while (std::getline(file, line))
    {
        tdev::SET_BENCH("t");
        static constexpr size_t WORD_LENGTH = 14;
        size_t i = WORD_LENGTH;
        for (; i < line.length(); ++i)
        {
            if (is_distinct_str(line.substr(i-WORD_LENGTH, WORD_LENGTH)))
                break;
        }
        std::cout << i << "\n";
        tdev::LOG_BENCH("t");
    }
    return 0;
}

