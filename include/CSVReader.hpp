#pragma once

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CSVReader
{
public:
    CSVReader() = default;
    static std::map<std::string, int> ReadFile(const std::string &file_name);
};
