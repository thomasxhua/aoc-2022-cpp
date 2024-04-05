#include <iostream>
#include <fstream>
#include <string>

struct Clock
{
    int cycle               = 1;
    int signal_strength_sum = 0;
    int x                   = 1;

    void increase_cycle()
    {
        ++cycle;
        maybe_update_signal_strength();
    }
    void noop()
    {
        increase_cycle();
    }
    bool is_updating_cycle()
    {
        return ((cycle-20) % 40) == 0;
    }
    void maybe_update_signal_strength()
    {
        if (!is_updating_cycle())
        {
            return;
        }
        signal_strength_sum += x * cycle;
    }
    void addx(const int _x)
    {
        increase_cycle();
        x += _x;
        increase_cycle();
    }
};

int main()
{
    std::fstream file("input");
    std::string line;
    Clock cock;
    while (std::getline(file, line))
    {
        if (line[0] == 'n')
        {
            cock.noop();
        }
        else if (line[0] == 'a')
        {
            cock.addx(std::stoll(line.substr(5)));
        }
    }
    std::cout << cock.signal_strength_sum << "\n";
    return 0;
}

