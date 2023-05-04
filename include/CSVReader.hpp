#pragma once

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class CSVReader
{
public:
    CSVReader() = default;
    static std::map<std::string, int> readFile(const std::string &filename);
};
