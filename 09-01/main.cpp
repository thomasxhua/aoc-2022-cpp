#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <set>
#include <cmath>

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
    if ((yh - yt) > 1)
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

    Pos h = {0,0};
    Pos t = {0,0};

    while (std::getline(file, line))
    {
        // parse move
        const Move m = str_to_move(line);
        // move head
        const auto& [D, N] = m;
        const Move new_m = {D,1};
        for (int i=0; i<N; ++i)
        {
            h = move_pos(h, new_m);
            // update tail
            t = update_tail(t, h);
            ts.insert(t);
        }
    }
    std::cout << "size: " << ts.size();
    return 0;
}

