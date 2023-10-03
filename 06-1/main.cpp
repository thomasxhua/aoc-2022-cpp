#include <iostream>
#include <fstream>
#include <string>

bool is_distinct_str(const std::string& str)
{
    for (int i=0; i<str.size(); ++i)
        for (int j=i+1; j<str.size(); ++j)
            if (str[i] == str[j])
                return false;
    return true;
}

int main()
{
    std::fstream file("input");
    std::string line;
    while (std::getline(file, line))
    {
        static constexpr size_t WORD_LENGTH = 4;
        size_t i = WORD_LENGTH;
        for (; i < line.length(); ++i)
        {
            if (is_distinct_str(line.substr(i-WORD_LENGTH, WORD_LENGTH)))
                break;
        }
        std::cout << i << "\n";
    }
    return 0;
}

