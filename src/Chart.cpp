#include "Chart.hpp"

Chart::Chart(Type type, const std::string &title, const std::string &x_axis_title, const std::string &y_axis_title, const std::string &axis_anchor)
{

    this->type = type;
    this->title = title;
    this->x_axis_title = x_axis_title;
    this->y_axis_title = y_axis_title;
    this->axis_anchor = axis_anchor;
}

Chart::Type Chart::typeFromString(const std::string &str)
{
    static std::map<std::string, Type> chartTypeMap = {
        {"bar", Type::BAR},
        {"line", Type::LINE},
        {"pie", Type::PIE}};
    return chartTypeMap[str];
}

Chart::Type Chart::getType() const
{
    return this->type;
}