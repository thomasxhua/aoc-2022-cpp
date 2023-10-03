#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

uint64_t count_ch_in_str(const char c, const std::string& str)
{
    uint64_t i = 0;
    for (const char d : str)
        i += (d == c);
    return i;
}

uint64_t idx_to_x_pos(const uint64_t idx)
{
    static uint64_t l = std::string("] [").length() + 1;
    return 1 + l * idx;
} 

struct Instr
{
    uint64_t n;
    uint64_t from, to;
    Instr(const std::string& str)
    {
        size_t i = 0;
        auto skip = [&]() { do { ++i; } while (str[i] != ' '); };
        skip();
        size_t n_start = ++i;
        skip();
        size_t n_stop = i;
        skip();
        size_t from_start = ++i;
        skip();
        size_t from_stop = i;
        skip();
        size_t to_start = ++i;
        skip();
        size_t to_stop = i;
        n       = std::stoi(str.substr(n_start, n_stop-n_start+1));
        from    = std::stoi(str.substr(from_start, from_stop-from_start+1)) - 1;
        to      = std::stoi(str.substr(to_start, to_stop-to_start+1)) - 1;
    }
    std::string to_string()
    {
        return std::to_string(n) + ", " + std::to_string(from) + ", " + std::to_string(to);
    }
};

struct State
{
    std::vector<std::stack<char>> stacks;
    uint64_t size = 0;

    State(const std::vector<std::string>& strs)
    {
        const uint64_t first_row = strs.size()-2;
        size = count_ch_in_str('[', strs[first_row]);
        for (uint64_t i=0; i<size; ++i)
        {
            stacks.push_back({});
        }
        // iterate through rows
        for (uint64_t i=first_row; i!=-1; --i)
        {
            for (uint64_t idx=0; idx<size; ++idx)
            {
                const char crate = strs[i][idx_to_x_pos(idx)];
                if (crate != ' ')
                    stacks[idx].push(crate);
            }
        }
    }

    std::string to_string()
    {
        std::string out = "";
        for (uint64_t i=0; i<size; ++i)
        {
            std::string line = "";
            auto stack = stacks[i];
            for (; !stack.empty(); stack.pop())
                line = std::string(1, stack.top()) + " " + line;
            line = std::to_string(i+1) + " " + line + "\n";
            out += line;
        }
        return out;
    }

    void do_instr(const Instr& instr)
    {
        for (uint64_t i=0; i<instr.n; ++i)
        {
            stacks[instr.to].push(stacks[instr.from].top());
            stacks[instr.from].pop();
        }
    }

    void do_instr_9001(const Instr& instr)
    {
        std::stack<char> temp;
        for (uint64_t i=0; i<instr.n; ++i)
        {
            temp.push(stacks[instr.from].top());
            stacks[instr.from].pop();
        }
        for (; !temp.empty(); temp.pop())
        {
            stacks[instr.to].push(temp.top());
        }
    }

    std::string generate_msg()
    {
        std::string out = "";
        for (const auto& stack : stacks)
            out += stack.top();
        return out;
    }
};

int main()
{
    std::fstream file("input");
    std::string line;
    // read info
    std::vector<std::string> state_strs;
    std::vector<std::string> instr_strs;
    bool is_instr = false;
    while (std::getline(file, line))
    {
        if (line == "")
            is_instr = !is_instr;
        else
            ((is_instr) ? instr_strs : state_strs).push_back(line);
    }
    // read state
    State state(state_strs);
    std::cout << state.to_string() << "-----\n";
    for (const auto& str : instr_strs)
    {
        Instr instr(str);
        state.do_instr_9001(instr);
    }
    std::cout << state.to_string() << "-----\n";
    std::cout << state.generate_msg();
    return 0;
}

