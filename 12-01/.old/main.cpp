#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <deque>

using Position  = std::pair<size_t,size_t>;
using Map       = std::vector<std::string>;

template <typename T>
using Matrix    = std::vector<std::vector<T>>;
using distance  = uint64_t;

#define DESTINATION 'E'
#define DESTINATION_HEIGHT 'z'
#define START 'S'
#define START_HEIGHT 'a'

#define EDGE_COST 1

#define DEBUG 0
#define DELOG(x) if (DEBUG) std::cout << "(DELOG) " << x << "\n"
#define PRINT(x) if (DEBUG) std::cout << x
#define PAUSE(x) if (DEBUG) std::cin.get()

std::string to_string(const Position& pos)
{
    return std::string("(") + (
        (pos.first == ~0ULL) ? "." : std::to_string(pos.first)
    ) + "," + (
        (pos.second == ~0ULL) ? "." : std::to_string(pos.second)
    ) + ")";
}

std::ostream& operator<<(std::ostream& target, const Position& pos)
{
    target << to_string(pos);
    return target;
};

template <typename T>
void init_matrix_with_map_dimensions(const Map& map, Matrix<T>& mat, const T e)
{
    std::for_each(map.begin(), map.end(), [&mat, &e](const std::string& str)
    {
        std::vector<T> v;
        std::for_each(str.begin(), str.end(), [&v, &e](const auto c) { v.push_back(e); });
        mat.push_back(v);
    });
}

void init_queue_with_map_dimensions(const Map& map, std::deque<Position>& queue)
{
    for (size_t i=0; i<map.size(); ++i)
    {
        for (size_t j=0; j<map[i].size(); ++j)
        {
            queue.push_back({i,j});
        }
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& target, const std::vector<T>& v)
{
    std::for_each(v.begin(), v.end(), [&](const T& ele)
    {
        target << ele << " ";
    });
    return target;
}

std::ostream& operator<<(std::ostream& target, const std::vector<uint64_t>& v)
{
    std::for_each(v.begin(), v.end(), [&](const uint64_t& ele)
    {
        if (ele == ~0ULL)
            target << "." << " ";
        else
            target << ele << " ";
    });
    return target;
}

template <typename T>
std::ostream& operator<<(std::ostream& target, const Matrix<T>& mat)
{
    std::for_each(mat.begin(), mat.end(), [&](const std::vector<T>& row) { target << row << "\n"; });
    return target;
};

template <typename T>
std::ostream& operator<<(std::ostream& target, const std::deque<T>& queue)
{
    for (auto it = queue.begin(); it != queue.end(); ++it)
    {
        target << *it << " ";
    }
    return target;
};

char translate_to_real_height(const char c)
{
    return (c == START) ? START_HEIGHT
        : (c == DESTINATION) ? DESTINATION_HEIGHT
        : c;
}

bool is_climbable(const Map& map, const Position& start, const Position& desti)
{
    const auto start_c = translate_to_real_height(map[start.first][start.second]);
    // is on map
    if (desti.first >= map.size()
        || desti.second >= map[desti.first].size())
    {
        return false;
    }
    const auto desti_c = translate_to_real_height(map[desti.first][desti.second]);
    // is in reach
    return (start_c-1 <= desti_c && desti_c <= start_c+1); 
}

std::vector<Position> generate_neighbors(const Map& map, const Position& p)
{
    std::vector<Position> v;
    const auto if_is_climbable_add = [&](const Position& delta)
    {
        const Position delta_p(p.first+delta.first, p.second+delta.second);
        if (is_climbable(map, p, delta_p))
        {
            v.push_back(delta_p);
        }
    };
    if_is_climbable_add({-1, 0});
    if_is_climbable_add({+1, 0});
    if_is_climbable_add({0, -1});
    if_is_climbable_add({0, +1});
    return v;
}

std::pair<Matrix<Position>, Matrix<uint64_t>> dijkstra(const Map& map, const Position& start, const Position& end)
{
    const size_t height = map.size();
    const size_t width  = map[0].size();
    // init algorithm
    Matrix<Position> prev;
    init_matrix_with_map_dimensions(map, prev, {-1,-1});
    Matrix<uint64_t> dist;
    init_matrix_with_map_dimensions(map, dist, ~0ULL);
    std::deque<Position> queue;
    init_queue_with_map_dimensions(map, queue);
    // init start
    dist[start.first][start.second] = 0;
    while (!queue.empty())
    {
        PRINT(dist);
        // determine min_pos
        auto min_pos_it = std::min_element(queue.begin(), queue.end(), [&dist](const Position& lhs, const Position& rhs)
        {
            return dist[lhs.first][lhs.second] < dist[rhs.first][rhs.second];
        });
        // delete it
        //std::cout << "[queue] " << queue << "\n";
        const Position min_pos = *min_pos_it;
        queue.erase(min_pos_it);
        DELOG("##### MIN: " << min_pos << " #####");
        // get neighbors
        const auto neighbors = generate_neighbors(map, min_pos);
        DELOG("neighbors: " << neighbors);
        for (const auto& neighbor : neighbors)
        {
            // if not exists skip
            auto neighbor_it = std::find(queue.begin(), queue.end(), neighbor);
            if (neighbor_it != queue.end())
            {
                uint64_t alt = dist[min_pos.first][min_pos.second] + EDGE_COST;
                if (alt < dist[neighbor.first][neighbor.second])
                {
                    dist[neighbor.first][neighbor.second] = alt;
                    prev[neighbor.first][neighbor.second] = min_pos;
                }
            }
            
        }
    PAUSE();
    }
    return {prev, dist};
}

std::vector<Position> find_shortest_path(const Map& map, const Position& start, const Position& end)
{
    std::vector<Position> path;
    auto [prev, dist] = dijkstra(map, start, end);
    std::transform(dist.begin((), dist.end(), 
    std::cout << dist;
    Position u = end;
    uint64_t i = 0;
    while (u != start)
    {
        path.insert(path.begin(), u);
        if (u.first == ~0ULL)
        {
            break;
        }
        u = prev[u.first][u.second];
    }
    return path;
}

/*
(.,.) (9,86) (9,87) (10,87) (11,87) (11,88) (11,89) (11,90) (11,91) (12,91) (12,92) (13,92) (13,93) (13,94) (13,95) (13,96) (13,97) (13,98) (13,99) (14,99) (15,99) (16,99) (17,99) (18,99) (19,99) (20,99) (21,99) (21,100) (22,100) (23,100) (24,100) (25,100) (26,100) (27,100) (27,99) (27,98) (27,97) (27,96) (27,95) (28,95) (28,94) (28,93) (29,93) (29,92) (29,91) (29,90) (29,89) (28,89) (28,88) (27,88) (26,88) (25,88) (25,87) (24,87) (24,86) (24,85) (24,84) (23,84) (22,84) (21,84) (20,84) (19,84) (18,84) (18,85) (18,86) (17,86) (16,86) (15,86) (14,86) (14,87) (14,88) (14,89) (14,90) (14,91) (14,92) (15,92) (15,93) (15,94) (15,95) (15,96) (15,97) (16,97) (17,97) (18,97) (19,97) (20,97) (21,97) (22,97) (23,97) (24,97) (25,97) (25,96) (25,95) (25,94) (25,93) (26,93) (26,92) (26,91) (26,90) (25,90) (25,89) (24,89) (23,89) (23,88) (22,88) (21,88) (21,89) (21,90) (20,90) (20,89) (20,88)
*/

int main()
{
    Map map;
    // get data
    std::fstream file("input");
    std::string line;
    while (std::getline(file, line))
    {
        map.push_back(line);
    }
    // find end
    Position end = {0,0};
    for (size_t i=0; i<map.size(); ++i)
        for (size_t j=0; j<map[i].size(); ++j)
            if (map[i][j] == DESTINATION)
                end = {i,j};
    // find start
    Position start = {0,0};
    for (size_t i=0; i<map.size(); ++i)
        for (size_t j=0; j<map[i].size(); ++j)
            if (map[i][j] == START)
                start = {i,j};
    DELOG("start " << start);
    DELOG("end " << end);
    // dijsktra
    const auto path = find_shortest_path(map, start, end);
    DELOG("##### PATH #####");
    DELOG(path);
    DELOG("size: " << path.size());
    // return count
    return 0;
}

