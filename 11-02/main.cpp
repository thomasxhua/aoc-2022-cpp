#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <numeric>

#define MONKEY_LINES_SIZE 7

#define TDEV_DELOG(x) std::cout << x << std::endl;

#if 0
int char_to_digit(const char c)
{
    return c - '0';
}

char digit_to_char(const int n)
{
    return n + '0';
}

constexpr uint64_t BIGGER_THAN_10_TO_19 = 1ULL << 63;

class BigInt
{
private:
    BigInt()
    {}
    std::vector<char> digits;
public:
    BigInt(const uint64_t n)
    {
        const std::string str = std::to_string(n);
        for (const auto c : str)
        {
            digits.insert(digits.begin(), c);
        }
    }
    BigInt(const BigInt& other)
    {
        digits = other.digits;
    }
    BigInt operator+(const BigInt& rhs) const
    {
        const size_t lhs_size   = digits.size();
        const size_t rhs_size   = rhs.digits.size();
        const size_t bigger     = std::max(lhs_size, rhs_size);
        BigInt res;
        bool was_carry = false;
        for (int i=0; i<bigger; ++i)
        {
            int now     = was_carry;
            was_carry   = false;
            const char lhs_char = (i < digits.size())       ? digits[i]     : digit_to_char(0);
            const char rhs_char = (i < rhs.digits.size())   ? rhs.digits[i] : digit_to_char(0);
            const int lhs_digit = char_to_digit(lhs_char);
            const int rhs_digit = char_to_digit(rhs_char);
            // res
            now += lhs_digit + rhs_digit;
            const char res_char = digit_to_char(now % 10);
            const bool res_carr = now > 9;
            // put
            res.digits.push_back(res_char);
            was_carry = res_carr;
            //std::cout << lhs_char << "," << rhs_char << " -> " << std::boolalpha << was_carry << "\n";
        }
        if (was_carry)
        {
            res.digits.push_back(digit_to_char(1));
        }
        return res;
    }
    BigInt& operator+=(const BigInt& rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    BigInt& operator++()
    {
        *this += 1;
        return *this;
    }
    BigInt operator*(const BigInt& rhs) const
    {
        BigInt res(0);
        for (BigInt counter(0); counter < rhs; counter += BigInt(1))
        {
            res += *this;
        }
        return res;
    }
    BigInt& operator*=(const BigInt& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    BigInt right_minus(const BigInt& rhs) const
    {
        if (*this < rhs)
        {
            throw std::exception();
        }
        if (*this == rhs)
        {
            return BigInt(0);
        }
        return BigInt(23342565);
        const size_t lhs_size   = digits.size();
        const size_t rhs_size   = rhs.digits.size();
        const size_t bigger     = std::max(lhs_size, rhs_size);
        BigInt res(*this);
        bool has_carry = false;
        for (int i=0; i<bigger; ++i)
        {
            int mini_res = 0;
            if (has_carry)
            {
                mini_res    = -1;
                has_carry   = false;
            }
            const char lhs_char = (i < digits.size())       ? digits[i]     : digit_to_char(0);
            const char rhs_char = (i < rhs.digits.size())   ? rhs.digits[i] : digit_to_char(0);
            const int lhs_digit = char_to_digit(lhs_char);
            const int rhs_digit = char_to_digit(rhs_char);
            mini_res += lhs_digit - rhs_digit;
            const char res_char = digit_to_char(std::abs(mini_res));
            res.digits[i] = res_char;
            has_carry = (mini_res < 0);
        }
        res.clean_zeros();
        return res;
    }
    void clean_zeros()
    {
        while (digits.size() > 1)
        {
            if (char_to_digit(digits[digits.size()-1]) == 0)
            {
                digits.pop_back();
            }
            else
            {
                break;
            }
        }
    }
    uint64_t to_int() const
    {
        if (digits.size() >= 20)
        {
            return 0;
        }
        uint64_t res = 0;
        for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        {
            res += *it;
            if (std::next(it, 1) != digits.rend())
            {
                res *= 10;
            }
        }
        return res;
    }
    BigInt operator%(const BigInt& rhs) const
    {
        // cheating
        if (digits.size() < 20)
        {
            const int n_this    = this->to_int();
            const int n_rhs     = rhs.to_int();
            return BigInt(n_this % n_rhs);
        }
        //BigInt fake_this(*this);
        //auto& a = fake_this.digits[this->digits.size()-1];
        //auto& b = fake_this.digits[this->digits.size()-2];
        //const int _a = char_to_digit(a);
        //const int _b = char_to_digit(b);
        //const int _c = _a*10 + _b;
        //const int _rhs = rhs.to_int();
        //if (_c > _rhs)
        //{
        //    BigInt c(_c - _rhs);
        //    if (c.size() == 1)
        //    {
        //        digits.pop_back();
        //        b = c.digits[0];
        //    }
        //    a = c.digits[1];
        //    b = c.digits[0];
        //    return (fake_this % rhs);
        //}
        return (this->right_minus(BIGGER_THAN_10_TO_19)) % rhs;
        //if (*this < rhs)
        //{
        //    return *this;
        //}
        //if (*this == rhs)
        //{
        //    return BigInt(0);
        //}
        //BigInt fake_this(*this);
        //for (; fake_this>=rhs; fake_this = fake_this.right_minus(rhs))
        //{}
        //return fake_this;
    }
    bool operator==(const BigInt& rhs) const
    {
        return digits == rhs.digits;
    }
    bool operator!=(const BigInt& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator<(const BigInt& rhs) const
    {
        if (digits.size() < rhs.digits.size())
        {
            return true;
        }
        if (digits.size() > rhs.digits.size())
        {
            return false;
        }
        for (size_t i=0; i<digits.size(); ++i)
        {
            const size_t idx = digits.size()-i-1;
            if (digits[idx] > rhs.digits[idx])
            {
                return false;
            }
        }
        if (*digits.begin() == *rhs.digits.begin())
        {
            return false;
        }
        return true;
    }
    bool operator<=(const BigInt& rhs) const
    {
        return (*this == rhs) || (*this < rhs);
    }
    bool operator>(const BigInt& rhs) const
    {
        return (rhs < *this);
    }
    bool operator>=(const BigInt& rhs) const
    {
        return (*this == rhs) || (*this > rhs);
    }
    std::string to_string() const
    {
        std::string out = "";
        for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        {
            out += *it;
        }
        return out;
    }
    friend std::ostream& operator<<(std::ostream& target, const BigInt& source);
};

std::ostream& operator<<(std::ostream& target, const BigInt& source)
{
    target << source.to_string();
    return target;
};
#endif

using ItemType = uint64_t;

struct Monkey
{
    std::vector<ItemType> items;
    std::function<void(ItemType&)> operation;
    int test_num;
    Monkey* monkey_true;
    Monkey* monkey_false;

    uint64_t turns = 0;

    void inspect()
    {
        operation(items.front());
    }
    void bored(const int monkey_mod)
    {
        ItemType& n = items.front();
        //n = n / 3;
        n = n % monkey_mod;
    }
    void test()
    {
        ItemType n = items.front();
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
    void act(const int monkey_mod)
    {
        while (!items.empty())
        {
            ++turns;
            inspect();
            bored(monkey_mod);
            test();
        }
    }
    std::string to_string()
    {
        std::string out = "";
        for (const auto& n : items)
        {
            out += std::to_string(n) + ", ";
            //out += n.to_string() + ", ";
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
                    const int _n = std::stoll(str.substr(start_idx, i-start_idx));
                    const ItemType n = _n;
                    items.push_back(n);
                    is_capturing_digit = false;
                }
            }
        }
        if (is_capturing_digit)
        {
            const int _n = std::stoll(str.substr(start_idx));
            const ItemType n = _n;
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
            operation = [](ItemType& n) { n = n+n; };
        else if (is_self_operating && !is_addition)
            operation = [](ItemType& n) { n = n*n; };
        else if (!is_self_operating && is_addition)
            operation = [rhs](ItemType& n) { n = n+rhs; };
        else if (!is_self_operating && !is_addition)
            operation = [rhs](ItemType& n) { n = n*rhs; };
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
    // print
    const auto pretty = [&](const int i)
    {
        std::cout << "===== #" << i+1 << " =====\n";
        for (size_t i=0; i<monkeys.size(); ++i)
        {
            std::cout << "[" << i << "]: " << monkeys[i]->to_string() << " (turns: " << monkeys[i]->turns << ")\n"; 
        }
    };
    // DO STUFF
    const int ROUND_NUM = 10'000;
    const int MONKEY_MOD = std::accumulate(monkeys.begin(), monkeys.end(), 1,
        [](const int acc, const Monkey* rhs)
        {
            return acc * rhs->test_num;
        });
    std::cout << "MONKEY_MOD: " << MONKEY_MOD << "\n";
    std::string input = "1";
    for (int i=0; i<ROUND_NUM; ++i)
    {
        for (auto monkey : monkeys)
        {
            monkey->act(MONKEY_MOD);
        }
        if (input == std::to_string(i+1))
        {
            pretty(i);
            std::cout << "next stop: ";
            std::getline(std::cin, input);
            if (input.empty())
            {
                input = std::to_string(i+2);
            }
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
    std::vector<uint64_t> turnss;
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

