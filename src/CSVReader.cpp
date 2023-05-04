#include "CSVReader.hpp"

std::map<std::string, int> CSVReader::readFile(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::string absolute_path = std::filesystem::absolute(filename).string();
        throw std::runtime_error("Error opening file, cannot read file at '" + absolute_path + '\'');
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
