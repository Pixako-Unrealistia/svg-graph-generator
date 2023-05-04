#include "CSVReader.hpp"

std::map<std::string, int> CSVReader::ReadFile(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return {};
    }

    std::map<std::string, int> data;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        data[key] = std::stoi(value);
    }

    file.close();
    return data;
}
