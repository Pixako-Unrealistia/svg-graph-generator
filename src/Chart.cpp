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
        {"pie", Type::PIE},
        {"scatter", Type::SCATTER}};

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

std::string Chart::randomHexColor()
{
    float golden_ratio_conjugate = 0.618033988749895f;
    float saturation = 0.5f;
    float value = 0.95f;

    static float hue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    hue += golden_ratio_conjugate;
    hue = std::fmod(hue, 1.0f);

    std::uint8_t red, green, blue;

    if (hue < 1.0f / 3.0f)
    {
        red = static_cast<std::uint8_t>((hue * 3.0f) * value * 255);
        green = static_cast<std::uint8_t>(value * 255);
        blue = static_cast<std::uint8_t>(0);
    }
    else if (hue < 2.0f / 3.0f)
    {
        red = static_cast<std::uint8_t>(value * 255);
        green = static_cast<std::uint8_t>(((2.0f / 3.0f - hue) * 3.0f) * value * 255);
        blue = static_cast<std::uint8_t>(0);
    }
    else
    {
        red = static_cast<std::uint8_t>(value * 255);
        green = static_cast<std::uint8_t>(0);
        blue = static_cast<std::uint8_t>(((hue - 2.0f / 3.0f) * 3.0f) * value * 255);
    }

    std::ostringstream oss;
    oss << "#" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(red)
        << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(green)
        << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(blue);

    return oss.str();
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
        {Type::PIE, "Pie"},
        {Type::SCATTER, "Scatter"}};

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
        svg << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << bar_width << "\" height=\"" << bar_height << "\" fill=\"" << Chart::randomHexColor() << "\" />\n";
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
    svg << "<circle cx=\"250\" cy=\"250\" r=\"200\" fill=\"" << Chart::randomHexColor() << "\" stroke=\"black\" stroke-width=\"2\" />\n";

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
        svg << "<path d=\"M250,250 L" << x1 << "," << y1 << " A200,200 0 " << (angle > 180 ? 1 : 0) << ",1 " << x2 << "," << y2 << " Z\" fill=\"" << Chart::randomHexColor() << "\" />\n";
        // Write the text
        int x3 = 250 + 200 * std::cos((start_angle + angle / 2) * M_PI / 180);
        int y3 = 250 + 200 * std::sin((start_angle + angle / 2) * M_PI / 180);

        // Very efficient way to determine the font size using high-level advanced mathematics

        int font_size = 20;
        double percentage = static_cast<double>(it->second) / total * 100.0;

        if (percentage > 50)
        {
            font_size = 20;
        }
        else if (percentage <= 50 && percentage > 30)
        {
            font_size = 18;
        }
        else if (percentage <= 30 && percentage > 20)
        {
            font_size = 17;
        }
        else if (percentage <= 20 && percentage > 10)
        {
            font_size = 16;
        }
        else if (percentage <= 10 && percentage > 7)
        {
            font_size = 15;
        }
        else if (percentage <= 7 && percentage > 5)
        {
            font_size = 10;
        }
        else if (percentage <= 5 && percentage > 3)
        {
            font_size = 7;
        }
        else if (percentage <= 3 && percentage > 1)
        {
            font_size = 5;
        }
        else
        {
            font_size = 4;
        }

        svg << "<text x=\"" << x3 << "\" y=\"" << y3 << "\" text-anchor=\"middle\" font-size=\"" << font_size << "\">" << it->first << "</text>\n";
        start_angle = end_angle;
        remaining_angle -= angle;
        it++;
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
            svg << "<line x1=\"" << prev_x << "\" y1=\"" << prev_y << "\" x2=\"" << x << "\" y2=\"" << y << "\" stroke=\"" << Chart::randomHexColor() << "\" stroke-width=\"" << line_width << "\" />\n";
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

void Chart::printScatterChart(const int point_size, const std::string &output_filename)
{
    // Find the maximum value in the data
    int max_value = 0;
    int min_value = INT_MAX;
    for (const auto &[key, value] : this->dataset)
    {
        if (value > max_value)
        {
            max_value = value;
        }
        if (value < min_value)
        {
            min_value = value;
        }
    }

    // Dynamically determine the height and width of the chart
    int chart_height = max_value * 20 + 100;
    int chart_width = (this->dataset.size() - 1) * 50 + 100;

    // Calculate the slope and intercept of the line of best fit
    double x_sum = 0;
    double y_sum = 0;
    double xy_sum = 0;
    double x_squared_sum = 0;
    int n = 0;
    for (const auto &[key, value] : this->dataset)
    {
        double x = n;
        double y = value;
        x_sum += x;
        y_sum += y;
        xy_sum += x * y;
        x_squared_sum += x * x;
        n++;
    }
    double slope = (n * xy_sum - x_sum * y_sum) / (n * x_squared_sum - x_sum * x_sum);
    double intercept = (y_sum - slope * x_sum) / n;

    // Create the SVG document
    std::stringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << chart_width << "\" height=\"" << chart_height << "\" viewBox=\"0 0 " << chart_width << " " << chart_height << "\">\n";

    // Create the chart title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << this->title << "</text>\n";

    // Create the x-axis title
    svg << "<text x=\"" << chart_width / 2 << "\" y=\"" << chart_height - 10 << "\" text-anchor=\"middle\" font-size=\"20\">" << this->x_axis_title << "</text>\n";

    // Create the y-axis title
    svg << "<text x=\"10\" y=\"" << chart_height / 2 + 10 << "\" text-anchor=\"middle\" font-size=\"20\" transform=\"rotate(270, 10, " << chart_height / 2 << ")\">" << this->y_axis_title << "</text>\n";

    // Create the line of best fit
    double x1 = 0;
    double y1 = intercept;
    double x2 = this->dataset.size() - 1;
    double y2 = slope * x2 + intercept;
    svg << "<line x1=\"" << x1 * 50 + 50 << "\" y1=\"" << chart_height - y1 * 20 - 50 << "\" x2=\"" << x2 * 50 + 50 << "\" y2=\"" << chart_height - y2 * 20 - 50 << "\" stroke=\"red\" stroke-width=\"2\" />\n";

    // Create the points
    int x = 50;
    for (const auto &[key, value] : this->dataset)
    {
        int y = chart_height - value * 20 - 50;
        svg << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << point_size << "\" fill=\"" << Chart::randomHexColor() << "\" />\n";
        svg << "<text x=\"" << x << "\" y=\"" << y - 10 << "\" text-anchor=\"middle\" font-size=\"15\">" << key << " " << value << "</text>\n";
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

std::string Chart::getHistogramChartString()
{
    // Find the length of the longest label in the dataset
    size_t maxLabelLength = 0;
    for (const auto &entry : this->dataset)
    {
        maxLabelLength = std::max(maxLabelLength, entry.first.length());
    }

    // Print the chart with equal padding for all labels
    std::stringstream ss;
    auto it = this->dataset.begin();
    while (it != this->dataset.end())
    {
        ss << std::setw(maxLabelLength) << std::left << it->first << ": ";
        for (int i = 0; i < it->second; i++)
        {
            ss << "*";
        }

        ss << " " << it->second << std::endl;
        it++;
    }

    return ss.str();
}

void Chart::printHistogramChart()
{
    std::cout << this->getHistogramChartString();
}