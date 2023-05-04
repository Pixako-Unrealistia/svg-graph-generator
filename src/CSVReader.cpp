#include "CSVReader.hpp"

bool CSVReader::readFile(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::string absolute_path = std::filesystem::absolute(filename).string();
        throw std::runtime_error("Error opening file, cannot read file at '" + absolute_path + '\'');
    }

    OrderedMap<std::string, double> data;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        data.insert(std::make_pair(key, std::stod(value)));
    }

    file.close();
    this->data = data;

    return true;
}

OrderedMap<std::string, double> CSVReader::getData() const
{
    return this->data;
}