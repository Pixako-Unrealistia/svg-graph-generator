#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

class JsonReader
{
public:
    JsonReader() = default;

    /**
     * @brief Reads the contents of a JSON file and stores it as a string.
     *
     * @param filename The name of the JSON file to read.
     * @return True if the file was successfully read, false otherwise.
     */
    bool readFile(const std::string &filename);

    /**
     * @brief Gets the data from the JSON string and returns it as a vector of strings.
     *
     * The method looks for specific keys in the JSON string, extracts their values,
     * and returns them as a vector of strings in the following order:
     *   1. Chart_Type
     *   2. Chart_Title
     *   3. X_Axis_Title
     *   4. Y_Axis_Title
     *   5. Axis_Anchor
     *
     * @return A vector of strings containing the data extracted from the JSON string.
     */
    std::vector<std::string> getData() const;

private:
    std::string getValue(size_t pos, const std::string &key) const;

    std::string json_string_;
};