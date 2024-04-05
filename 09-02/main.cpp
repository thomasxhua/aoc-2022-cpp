#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <set>
#include <cmath>
#include <vector>

enum Direction
{
    _NO_DIRECTION,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

using Move  = std::pair<Direction,uint64_t>;
using Pos   = std::pair<int,int>;

Direction char_to_direction(const char c)
{
    switch (c)
    {
        case 'U':   return UP;
        case 'R':   return RIGHT;
        case 'D':   return DOWN;
        case 'L':   return LEFT;
        default:    return _NO_DIRECTION;
    }
}

Pos move_pos(const Pos p, const Move m)
{
    auto [x,y] = p;
    const auto [d,n] = m;
    switch (d)
    {
        case UP:    y -= n; break;
        case DOWN:  y += n; break;
        case LEFT:  x -= n; break;
        case RIGHT: x += n; break;
        default:    break;
    }
    return {x,y};
}

Pos update_tail(Pos t, Pos h)
{
    bool moved_x = false;
    bool moved_y = false;
    auto& [xt,yt] = t;
    auto& [xh,yh] = h;
    // check x
    if ((xh - xt) > 1)
    {
        ++xt;
        moved_x = true;
    }
    else if ((xh - xt) < -1)
    {
        --xt;
        moved_x = true;
    }
    // check y
    else if ((yh - yt) > 1)
    {
        ++yt;
        moved_y = true;
    }
    else if ((yh - yt) < -1)
    {
        --yt;
        moved_y = true;
    }
    // if diagonal, step closer
    if (xt != xh && yt != yh)
    {
        if (moved_x)
        {
            if ((yh - yt) > 0)
            {
                ++yt;
            }
            else if ((yh - yt) < 0)
            {
                --yt;
            }
        }
        else if (moved_y)
        {
            if ((xh - xt) > 0)
            {
                ++xt;
            }
            else if ((xh - xt) < 0)
            {
                --xt;
            }
        }
    }
    return t;
}

Move str_to_move(const std::string& str)
{
    const Direction d = char_to_direction(str[0]);
    const uint64_t n  = std::stoull(str.substr(2));
    return {d,n};
}

int main()
{
    std::fstream file("input");
    std::string line;

    std::set<Pos> ts;
    std::set<Pos> t2s;
    std::set<Pos> t3s;
    std::set<Pos> t4s;
    std::set<Pos> t5s;
    std::set<Pos> t6s;
    std::set<Pos> t7s;
    std::set<Pos> t8s;
    std::set<Pos> t9s;

    Pos h = {0,0};
    Pos t = {0,0};
    Pos t2 = {0,0};
    Pos t3 = {0,0};
    Pos t4 = {0,0};
    Pos t5 = {0,0};
    Pos t6 = {0,0};
    Pos t7 = {0,0};
    Pos t8 = {0,0};
    Pos t9 = {0,0};

    while (std::getline(file, line))
    {
        // parse move
        const Move m = str_to_move(line);
        const Move _ = {_NO_DIRECTION,0};

        // move head
        const auto& [D, N] = m;
        const Move new_m = {D,1};
        for (int i=0; i<N; ++i)
        {
            h = move_pos(h, new_m);
            // update tail
            t = update_tail(t, h);
            ts.insert(t);
            t2 = update_tail(t2, t);
            t3 = update_tail(t3, t2);
            t4 = update_tail(t4, t3);
            t5 = update_tail(t5, t4);
            t6 = update_tail(t6, t5);
            t7 = update_tail(t7, t6);
            t8 = update_tail(t8, t7);
            t9 = update_tail(t9, t8);
            t9s.insert(t9);
        }

    }
    std::cout << "size: " << ts.size() << "\n";
    std::cout << "size: " << t9s.size() << "\n";
    return 0;
}

