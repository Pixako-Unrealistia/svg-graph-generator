#include "JsonReader.hpp"

bool JsonReader::readFile(const std::string &filename)
{
    std::ifstream json_file(filename);

    if (!json_file.is_open())
    {
        std::string absolute_path = std::filesystem::absolute(filename).string();
        throw std::runtime_error("Error opening file, cannot read file at '" + absolute_path + '\'');
    }

    std::stringstream buffer;
    buffer << json_file.rdbuf();
    json_string_ = buffer.str();

    return true;
}

std::vector<std::string> JsonReader::getData() const
{
    std::vector<std::string> json_data;

    size_t pos = json_string_.find("Chart_Type");
    std::string chart_type = getValue(pos, "Chart_Type");
    json_data.push_back(chart_type);

    pos = json_string_.find("Chart_Title");
    std::string chart_title = getValue(pos, "Chart_Title");
    json_data.push_back(chart_title);

    pos = json_string_.find("X_Axis_Title");
    std::string x_axis_title = getValue(pos, "X_Axis_Title");
    json_data.push_back(x_axis_title);

    pos = json_string_.find("Y_Axis_Title");
    std::string y_axis_title = getValue(pos, "Y_Axis_Title");
    json_data.push_back(y_axis_title);

    pos = json_string_.find("Axis_Anchor");
    std::string axis_anchor = getValue(pos, "Axis_Anchor");
    json_data.push_back(axis_anchor);

    return json_data;
}

std::string JsonReader::getValue(size_t pos, const std::string &key) const
{
    const int OFFSET = key.size() + 3;
    std::string value = json_string_.substr(pos + OFFSET, json_string_.find(",", pos) - (pos + OFFSET));
    // Replace all quotes with empty strings
    size_t quote_pos = value.find("\"");
    while (quote_pos != std::string::npos)
    {
        value.replace(quote_pos, 1, "");
        quote_pos = value.find("\"");
    }
    // Remove all spaces
    size_t space_pos = value.find(" ");
    while (space_pos != std::string::npos)
    {
        value.replace(space_pos, 1, "");
        space_pos = value.find(" ");
    }

    return value;
}
