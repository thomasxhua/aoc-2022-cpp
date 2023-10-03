#include <iostream>
#include <fstream>
#include <string>

enum hand_t
{
    ROCK        = 1,
    PAPER       = 2,
    SCISSORS    = 3
};

uint64_t char_to_score(const char c, const char begin)
{
    return (c-begin) + 1;
}

enum result_t
{
    LOSE    = 0,
    DRAW    = 3,
    WIN     = 6
};

uint64_t char_to_result(const char c)
{
    return (c-'X') * 3;
}

uint64_t score(const char own, const char opp)
{
    uint64_t own_h  = char_to_score(own, 'X');
    uint64_t opp_h  = char_to_score(opp, 'A');
    if (own_h == opp_h)
        return DRAW;
    switch (own_h)
    {
        case ROCK:      return (opp_h == PAPER)     ? LOSE  : WIN;
        case PAPER:     return (opp_h == SCISSORS)  ? LOSE  : WIN;
        case SCISSORS:  return (opp_h == ROCK)      ? LOSE  : WIN;
        default:        return -1;
    }
}

uint64_t answer(const char opp, const char result)
{
    uint64_t opp_h      = char_to_score(opp, 'A');
    uint64_t result_h   = char_to_result(result);
    if (result_h == DRAW)
        return opp_h;
    switch (opp_h)
    {
        case ROCK:      return (result_h == LOSE)   ? SCISSORS : PAPER;
        case PAPER:     return (result_h == LOSE)   ? ROCK     : SCISSORS;
        case SCISSORS:  return (result_h == LOSE)   ? PAPER    : ROCK;
        default:        return -1;
    }
}

int main()
{
    std::fstream file("input");
    std::string line;
    uint64_t total_score = 0;
    while (std::getline(file, line))
    {
        char opp        = line[0];
        char result     = line[2];
        uint64_t ans    = answer(opp, result);
        total_score += char_to_result(result) + ans;
    }
    std::cout << total_score;
    return 0;
}

