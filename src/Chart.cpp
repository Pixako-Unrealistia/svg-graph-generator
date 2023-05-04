#include "Chart.hpp"

Chart::Chart(
    Type type,
    const std::string &title,
    const std::string &x_axis_title,
    const std::string &y_axis_title,
    const std::string &axis_anchor,
    const OrderedMap<std::string, double> &dataset)
{

    this->type = type;
    this->title = title;
    this->x_axis_title = x_axis_title;
    this->y_axis_title = y_axis_title;
    this->axis_anchor = axis_anchor;
    this->dataset = dataset;
}

Chart::Type Chart::typeFromString(const std::string &str)
{
    static OrderedMap<std::string, Type> chartTypeMap = {
        {"bar", Type::BAR},
        {"line", Type::LINE},
        {"pie", Type::PIE}};

    auto it = chartTypeMap.find(str);
    if (it != chartTypeMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::invalid_argument("Invalid chart type string: " + str);
    }
}

Chart::Type Chart::getType() const
{
    return this->type;
}

std::string Chart::getTypeString() const
{
    static OrderedMap<Type, std::string> chartTypeMap = {
        {Type::BAR, "Bar"},
        {Type::LINE, "Line"},
        {Type::PIE, "Pie"}};

    auto it = chartTypeMap.find(this->type);
    if (it != chartTypeMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::invalid_argument("Invalid chart type: " + std::to_string(static_cast<int>(this->type)));
    }
}

void Chart::printBarChart(const int bar_width, const int bar_gap, const std::string &output_filename)
{
    // Find the maximum value in the data
    int max_value = 0;
    for (const auto &[key, value] : this->dataset)
    {
        // std::cout << key << " " << value << std::endl;
        // std::cout << "max_value: " << max_value << std::endl;
        if (value > max_value)
        {
            max_value = value;
        }
    }

    // Dynamically determine the height of the chart
    int chart_height = max_value * 20 + 100;
    int chart_width = (bar_width + bar_gap) * this->dataset.size() + bar_gap + 100;

    // Create the SVG document
    std::stringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << chart_width << "\" height=\"" << chart_height << "\" viewBox=\"0 0 " << chart_width << " " << chart_height << "\">\n";

    // Create the chart title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << this->title << "</text>\n";

    // Create the x-axis title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"" << chart_height - 10 << "\" text-anchor=\"middle\" font-size=\"20\">" << this->x_axis_title << "</text>\n";

    // Create the y-axis title
    svg << "<text x=\"10\" y=\"" << chart_height / 2 + 10 << "\" text-anchor=\"middle\" font-size=\"20\" transform=\"rotate(270, 10, " << chart_height / 2 << ")\">" << this->y_axis_title << "</text>\n";

    // Create the bars
    int x = bar_gap + 50;
    for (const auto &[key, value] : this->dataset)
    {
        int bar_height = value * 20;
        int y = chart_height - bar_height - 50;
        svg << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << bar_width << "\" height=\"" << bar_height << "\" fill=\"blue\" />\n";
        x += bar_width + bar_gap;

        // Create the label
        svg << "<text x=\"" << x - bar_width / 2 - bar_gap << "\" y=\"" << chart_height - 35 << "\" text-anchor=\"middle\" font-size=\"15\">" << key << "</text>\n";
    }

    svg << "</svg>";

    // Write the SVG document to a file
    std::ofstream output_file(output_filename);
    if (output_file.is_open())
    {
        output_file << svg.str();
        output_file.close();
    }
    else
    {
        std::cerr << "Error: could not write to file " << output_filename << std::endl;
    }
}

void Chart::printPieChart(const std::string &output_filename)
{

    // Calculate the value into percentages
    int total = 0;
    for (const auto &[key, value] : this->dataset)
    {
        total += value;
    }

    // Create the SVG document
    std::stringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"500\" height=\"500\" viewBox=\"0 0 500 500\">\n";

    // Create the chart title
    svg << "<text x=\"250\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << this->title << "</text>\n";

    // Create the circle
    svg << "<circle cx=\"250\" cy=\"250\" r=\"200\" fill=\"blue\" stroke=\"black\" stroke-width=\"2\" />\n";

    // Create the pie chart
    int start_angle = 0;
    int remaining_angle = 360;
    auto it = this->dataset.begin();
    while (it != this->dataset.end())
    {
        int angle = it->second * 360 / total;
        if (std::next(it) == this->dataset.end())
        {
            angle = remaining_angle;
        }
        int end_angle = start_angle + angle;
        int x1 = 250 + 200 * std::cos(start_angle * M_PI / 180);
        int y1 = 250 + 200 * std::sin(start_angle * M_PI / 180);
        int x2 = 250 + 200 * std::cos(end_angle * M_PI / 180);
        int y2 = 250 + 200 * std::sin(end_angle * M_PI / 180);
        svg << "<path d=\"M250,250 L" << x1 << "," << y1 << " A200,200 0 " << (angle > 180 ? 1 : 0) << ",1 " << x2 << "," << y2 << " Z\" fill=\"blue\" />\n";
        // write the text
        int x3 = 250 + 200 * std::cos((start_angle + angle / 2) * M_PI / 180);
        int y3 = 250 + 200 * std::sin((start_angle + angle / 2) * M_PI / 180);
        svg << "<text x=\"" << x3 << "\" y=\"" << y3 << "\" text-anchor=\"middle\" font-size=\"20\">" << it->first << "</text>\n";
        start_angle = end_angle;
        remaining_angle -= angle;
        it++;
    }

    // Write the SVG document to a file
    std::ofstream output_file(output_filename);
    if (output_file.is_open())
    {
        output_file << svg.str();
        output_file.close();
    }
    else
    {
        std::cerr << "Error: could not write to file " << output_filename << std::endl;
    }
}

void Chart::printLineChart(const int line_width, const std::string &output_filename)
{
    // Find the maximum value in the data
    int max_value = 0;
    for (const auto &[key, value] : this->dataset)
    {
        if (value > max_value)
        {
            max_value = value;
        }
    }

    // Dynamically determine the height and width of the chart
    int chart_height = max_value * 20 + 100;
    int chart_width = (this->dataset.size() - 1) * 50 + 100;

    // Create the SVG document
    std::stringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << chart_width << "\" height=\"" << chart_height << "\" viewBox=\"0 0 " << chart_width << " " << chart_height << "\">\n";

    // Create the chart title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << this->title << "</text>\n";

    // Create the x-axis title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"" << chart_height - 10 << "\" text-anchor=\"middle\" font-size=\"20\">" << this->x_axis_title << "</text>\n";

    // Create the y-axis title
    svg << "<text x=\"10\" y=\"" << chart_height / 2 + 10 << "\" text-anchor=\"middle\" font-size=\"20\" transform=\"rotate(270, 10, " << chart_height / 2 << ")\">" << this->y_axis_title << "</text>\n";

    // Create the lines
    int x = 50;
    int prev_x = 0, prev_y = 0;
    for (const auto &[key, value] : this->dataset)
    {
        int y = chart_height - value * 20 - 50;
        if (prev_x != 0 && prev_y != 0)
        {
            svg << "<line x1=\"" << prev_x << "\" y1=\"" << prev_y << "\" x2=\"" << x << "\" y2=\"" << y << "\" stroke=\"blue\" stroke-width=\"" << line_width << "\" />\n";
        }
        // svg << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << line_width + 2 << "\" fill=\"white\" stroke=\"blue\" stroke-width=\"" << line_width << "\" />\n";
        svg << "<text x=\"" << x << "\" y=\"" << y - 10 << "\" text-anchor=\"middle\" font-size=\"15\">" << key << " " << value << "</text>\n";
        prev_x = x;
        prev_y = y;
        x += 50;
    }

    svg << "</svg>";

    // Write the SVG document to a file
    std::ofstream output_file(output_filename);
    if (output_file.is_open())
    {
        output_file << svg.str();
        output_file.close();
    }
    else
    {
        std::cerr << "Error: could not write to file " << output_filename << std::endl;
    }
}

void Chart::printHistogramChart()
{
    // Find the length of the longest label in the dataset
    size_t maxLabelLength = 0;
    for (const auto &entry : this->dataset)
    {
        maxLabelLength = std::max(maxLabelLength, entry.first.length());
    }

    // Print the chart with equal padding for all labels
    auto it = this->dataset.begin();
    while (it != this->dataset.end())
    {
        std::cout << std::setw(maxLabelLength) << std::left << it->first << ": ";
        for (int i = 0; i < it->second; i++)
        {
            std::cout << "*";
        }

        std::cout << " " << it->second << std::endl;
        it++;
    }
}