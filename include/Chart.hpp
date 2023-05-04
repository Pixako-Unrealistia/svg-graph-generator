#pragma once

#include <iostream>
#include <string>
#include <map>

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
     * @param axis_anchor The anchor point for the axis. This determines where the axis will be positioned on the chart. This should be a string value of "start", "middle", or "end".
     */
    Chart(Type type, const std::string &title, const std::string &x_axis_title, const std::string &y_axis_title, const std::string &axis_anchor);

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

private:
    Type type;
    std::string title;
    std::string x_axis_title;
    std::string y_axis_title;
    std::string axis_anchor;
};
