#pragma once

#include "OrderedMap.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

class Chart
{
public:
    /**
     * @brief The Type enum class represents the different chart types.
     */
    enum class Type
    {
        BAR,
        LINE,
        PIE
    };

    /**
     * @brief Constructs a new chart object of the specified type with the given title, x-axis title, y-axis title, and axis anchor.
     *
     * @param type The type of the chart.
     * @param title The title of the chart.
     * @param x_axis_title The title of the x-axis.
     * @param y_axis_title The title of the y-axis.
     * @param axis_anchor The anchor point for the axis. This determines where the axis will be positioned on the chart.
     * @param dataset The data to be plotted on the chart.
     */
    Chart(Type type, const std::string &title, const std::string &x_axis_title, const std::string &y_axis_title, const std::string &axis_anchor, const OrderedMap<std::string, int> &dataset);

    /**
     * @brief Converts a string representation of a chart type to its enum class representation.
     *
     * @param str The string representation of the chart type.
     * @return The enum class representation of the chart type.
     */
    static Type typeFromString(const std::string &str);

    /**
     * @brief Gets the type of the chart.
     *
     * @return The type of the chart.
     */
    Type getType() const;

    /**
     * @brief Prints bar charts
     *
     * @param bar_width The width of each bar in the chart.
     * @param bar_gap The gap between each bar in the chart.
     * @param output_filename The name of the output file.
     */
    void printBarChart(const int bar_width, const int bar_gap, const std::string &output_filename);

    /**
     * @brief Prints Pie charts
     *
     * @param output_filename The name of the output file.
     */
    void printPieChart(const std::string &output_filename);

    /**
     * @brief Prints line charts
     *
     * @param line_width The width of each line in the chart.
     * @param output_filename The name of the output file.
     */
    void printLineChart(const int line_width, const std::string &output_filename);

    /**
     * @brief Prints histogram charts
     *
     */
    void printHistogramChart();

private:
    Type type;
    std::string title;
    std::string x_axis_title;
    std::string y_axis_title;
    std::string axis_anchor;
    OrderedMap<std::string, int> dataset;
};
