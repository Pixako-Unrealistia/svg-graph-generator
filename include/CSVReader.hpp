#pragma once

#include "OrderedMap.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class CSVReader
{
public:
    CSVReader() = default;
    static OrderedMap<std::string, int> readFile(const std::string &filename);
};
