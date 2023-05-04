#pragma once

#include "OrderedMap.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

class CSVReader
{
public:
    CSVReader() = default;

    /**
     * @brief Reads the contents of a CSV file and stores it as a OrderedMap<std::string, double>.
     *
     * @param filename The name of the CSV file to read.
     * @return True if the file was successfully read, false otherwise.
     */
    bool readFile(const std::string &filename);

    /**
     * @brief Gets the data from the CSV string and returns it as a OrderedMap<std::string, double>.
     *
     * @return A OrderedMap<std::string, double> containing the data extracted from the CSV string.
     */
    OrderedMap<std::string, double> getData() const;

private:
    OrderedMap<std::string, double> data;
};