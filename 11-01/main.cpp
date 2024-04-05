#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>

#define MONKEY_LINES_SIZE 7

struct Monkey
{
    std::vector<int> items;
    std::function<void(int&)> operation;
    int test_num;
    Monkey* monkey_true;
    Monkey* monkey_false;

    int turns = 0;

    void inspect()
    {
        operation(items.front());
    }
    void bored()
    {
        int& n = items.front();
        n = n / 3;
    }
    void test()
    {
        const auto n = items.front();
        items.erase(items.begin());
        if (n % test_num == 0)
        {
            monkey_true->items.push_back(n);
        }
        else
        {
            monkey_false->items.push_back(n);
        }
    }
    void act()
    {
        while (!items.empty())
        {
            ++turns;
            inspect();
            bored();
            test();
        }
    }
    std::string to_string()
    {
        std::string out = "";
        for (const auto& n : items)
        {
            out += std::to_string(n) + ", ";
        }
        return out;
    }
    void init_start_items(const std::string& str)
    {
        bool is_capturing_digit = false;
        size_t start_idx        = 0;
        for (size_t i=0; i<str.size(); ++i)
        {
            const auto c        = str[i];
            const bool is_digit = std::isdigit(c);
            if (!is_capturing_digit)
            {
                if (is_digit)
                {
                    is_capturing_digit  = true;
                    start_idx           = i;
                }
            }
            else
            {
                if (!is_digit)
                {
                    const int n = std::stoll(str.substr(start_idx, i-start_idx));
                    items.push_back(n);
                    is_capturing_digit = false;
                }
            }
        }
        if (is_capturing_digit)
        {
            const int n = std::stoll(str.substr(start_idx));
            items.push_back(n);
        }
    }
    void init_operation(const std::string& str)
    {
        const std::string search = "old ";
        size_t i = 0;
        for (; i<str.size(); ++i)
        {
            if (str.substr(i, search.size()) == search)
            {
                break;
            }
        }
        const std::string op_str = str.substr(i+search.size());
        const bool is_self_operating    = !std::isdigit(op_str[2]);
        const bool is_addition          = op_str[0] == '+';
        int rhs = 0;
        if (!is_self_operating)
        {
            rhs = std::stoll(op_str.substr(2));
        }
        if (is_self_operating && is_addition)
            operation = [](int& n) { n = n+n; };
        else if (is_self_operating && !is_addition)
            operation = [](int& n) { n = n*n; };
        else if (!is_self_operating && is_addition)
            operation = [rhs](int& n) { n = n+rhs; };
        else if (!is_self_operating && !is_addition)
            operation = [rhs](int& n) { n = n*rhs; };
    }
    void init_test(const std::string& str)
    {
        for (size_t i=0; i<str.size(); ++i)
        {
            const auto c = str[i];
            if (std::isdigit(c))
            {
                test_num = std::stoll(str.substr(i-1));
                break;
            }
        }
    }
    void init_test_monkeys(const std::string& str0, const std::string& str1, const std::vector<Monkey*> monkeys)
    {
        size_t monkey_num_0;
        size_t monkey_num_1;
        for (size_t i=0; i<str0.size(); ++i)
        {
            const auto c = str0[i];
            if (std::isdigit(c))
            {
                monkey_num_0    = std::stoll(str0.substr(i-1));
                monkey_true     = monkeys[monkey_num_0];
                break;
            }
        }
        for (size_t i=0; i<str1.size(); ++i)
        {
            const auto c = str1[i];
            if (std::isdigit(c))
            {
                monkey_num_1    = std::stoll(str1.substr(i-1));
                monkey_false    = monkeys[monkey_num_1];
                break;
            }
        }
    }
    void init_from_lines(const int i, const std::vector<std::string>& lines, const std::vector<Monkey*>& monkeys)
    {
        const int start_idx = (i * MONKEY_LINES_SIZE);
        init_start_items(lines[start_idx + 1]);
        init_operation(lines[start_idx + 2]);
        init_test(lines[start_idx + 3]);
        init_test_monkeys(lines[start_idx + 4], lines[start_idx + 5], monkeys);
    }
};

int main()
{
    // get text
    std::fstream file("input");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    const int num_of_monkeys = (lines.size() + 1) / MONKEY_LINES_SIZE;
    // count monkeys
    std::vector<Monkey*> monkeys;
    // init monkeys
    for (int i=0; i<num_of_monkeys; ++i)
    {
        monkeys.push_back(new Monkey());
    }
    // set up monkeys
    for (int i=0; i<num_of_monkeys; ++i)
    {
        monkeys[i]->init_from_lines(i, lines, monkeys);
    }
    const int ROUND_NUM = 20;
    for (int i=0; i<ROUND_NUM; ++i)
    {
        for (auto monkey : monkeys)
        {
            monkey->act();
        }
#if 0
        // print
        for (size_t i=0; i<monkeys.size(); ++i)
        {
            std::cout << "[" << i << "]: " << monkeys[i]->to_string() << "\n"; 
        }
#endif
    }
    // count
    std::vector<int> turnss;
    for (const auto monkey : monkeys)
    {
        turnss.push_back(monkey->turns);
    }
    std::sort(turnss.begin(), turnss.end());
    std::cout << "\n-----\n" << turnss[turnss.size()-1] * turnss[turnss.size()-2] << "\n";
    // clean
    for (auto monkey : monkeys)
    {
        delete monkey;
    }
    return 0;
}

#if 0
    // set up monkeys
    Monkey monkey_0, monkey_1, monkey_2, monkey_3;
    // 0
    monkey_0.items.insert(monkey_0.items.end(), {79, 98});
    monkey_0.operation      = [](int& n) { n = n * 19; };
    monkey_0.test_num       = 23;
    monkey_0.monkey_true    = &monkey_2;
    monkey_0.monkey_false   = &monkey_3;
    // 1
    monkey_1.items.insert(monkey_1.items.end(), {54, 65, 75, 74});
    monkey_1.operation      = [](int& n) { n = n + 6; };
    monkey_1.test_num       = 19;
    monkey_1.monkey_true    = &monkey_2;
    monkey_1.monkey_false   = &monkey_0;
    // 2
    monkey_2.items.insert(monkey_2.items.end(), {79, 60, 97});
    monkey_2.operation      = [](int& n) { n = n * n; };
    monkey_2.test_num       = 13;
    monkey_2.monkey_true    = &monkey_1;
    monkey_2.monkey_false   = &monkey_3;
    // 3
    monkey_3.items.insert(monkey_3.items.end(), {74});
    monkey_3.operation      = [](int& n) { n = n + 3; };
    monkey_3.test_num       = 17;
    monkey_3.monkey_true    = &monkey_0;
    monkey_3.monkey_false   = &monkey_1;
    // do stuff
    const int ROUND_NUM = 20;
    for (int i=0; i<ROUND_NUM; ++i)
    {
        monkey_0.act();
        monkey_1.act();
        monkey_2.act();
        monkey_3.act();
#endif
#if 0
        std::cout << "-----\n";
        std::cout << "[0]: " << monkey_0.to_string() << " (" << std::to_string(monkey_0.turns) << ")\n"; 
        std::cout << "[1]: " << monkey_1.to_string() << " (" << std::to_string(monkey_1.turns) << ")\n";
        std::cout << "[2]: " << monkey_2.to_string() << " (" << std::to_string(monkey_2.turns) << ")\n";
        std::cout << "[3]: " << monkey_3.to_string() << " (" << std::to_string(monkey_3.turns) << ")\n";
#endif
#if 0
    }

    std::vector<int> turnss;
    turnss.push_back(monkey_0.turns);
    turnss.push_back(monkey_1.turns);
    turnss.push_back(monkey_2.turns);
    turnss.push_back(monkey_3.turns);
    std::sort(turnss.begin(), turnss.end());
    std::cout << "\n-----\n" << turnss[3] * turnss[2] << "\n";
#endif

