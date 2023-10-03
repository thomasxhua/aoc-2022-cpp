#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <functional>

// total time: ~1h

static constexpr size_t TOTAL   = 70'000'000;
static constexpr size_t NEEDED  = 30'000'000;

struct FSNode
{
    const std::string id;

    size_t size     = 0;
    size_t depth    = 0;
    FSNode* parent  = nullptr;

    std::vector<FSNode*> children;

    FSNode(
        const std::string& _id,
        FSNode* _parent)
        : id(_id), parent(_parent)
    {
        depth = (parent) ? parent->depth + 1 : 0;
    }

    void print()
    {
        for (size_t i=0; i<depth; ++i)
            std::cout << "  ";
        std::cout << "- " + id << " (" << size << ")\n";
        for (const auto& child : children)
            child->print();
    }

    void operator<<(const uint64_t _size)
    {
        size += _size;
        if (parent)
            *parent << _size;
    }

    size_t sum_filter(const std::function<bool(FSNode*)>& pred)
    {
        size_t sum = 0;
        for (const auto& child : children)
        {
            if (pred(child))
                sum += child->size;
            sum += child->sum_filter(pred);
        }
        return sum;
    }

    size_t min_filter(const std::function<bool(FSNode*)>& pred)
    {
        size_t current_min = size;
        for (const auto& child : children)
        {
            if (pred(child))
            {
                size_t child_min = child->min_filter(pred);
                if (child_min < current_min)
                    current_min = child_min;
            }
        }
        return current_min;
    }

};

int main()
{
    std::fstream file("input");
    std::string line;

    FSNode* root = new FSNode("", nullptr);
    root->children.push_back(new FSNode("/", root));
    FSNode* current = root;
    while (std::getline(file, line))
    {
        static constexpr uint64_t IDX_SHELL     = 0;
        static constexpr uint64_t IDX_CMD       = 2;
        static constexpr uint64_t IDX_CD_DIR    = 5;
        static constexpr uint64_t IDX_LS_DIR    = 4;
        if (line[IDX_SHELL] == '$')
        {
            if (line[IDX_CMD] == 'c')
            {
                if (line[IDX_CD_DIR] == '.')
                {
                    current = current->parent;
                }
                else
                {
                    std::string dir_id = line.substr(IDX_CD_DIR, line.size() - IDX_CD_DIR);
                    for (const auto& child : current->children)
                    {
                        if (child->id == dir_id)
                        {
                            child->parent   = current;
                            current         = child;
                        }
                    }
                }
            }
        }
        else if (line[IDX_SHELL] == 'd')
        {
            std::string dir_id = line.substr(IDX_LS_DIR, line.size() - IDX_LS_DIR);
            current->children.push_back(new FSNode(dir_id, current));
        }
        else
        {
            size_t i = 0;
            for (; i < line.length() && line[i] != ' '; ++i)
            {}
            const size_t size = std::stoi(line.substr(0, i));
            *current << size;
        }
    }
    root->print();
    std::cout << root->min_filter([&](FSNode* node) -> bool { return node->size >= (root->size - (TOTAL - NEEDED)); });
    return 0;
}

