#include "OrderedMap.hpp"

#include <list>
#include <string>
#include <stdexcept>

int main(int argc, char *argv[])
{
    // Compare OrderedMap orders with list, see if the order is the same
    OrderedMap<std::string, double> orderedMap;

    std::list<std::string> list = {
        "Nyan",
        "Neko",
        "Kawaii",
        "Yuzu",
        "Nyannnn"};

    for (const auto &item : list)
    {
        orderedMap.insert(std::make_pair(item, 12.34));
    }

    std::list<std::string> orderedMapOrder;
    for (const auto &item : orderedMap)
    {
        orderedMapOrder.push_back(item.first);
    }

    if (list == orderedMapOrder)
    {
        return 0;
    }
    else
    {
        throw std::runtime_error("OrderedMap order is not the same as list order");
        return 1;
    }
}