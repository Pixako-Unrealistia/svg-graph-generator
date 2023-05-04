
#include "JsonReader.hpp"
#include "CSVReader.hpp"
#include "Chart.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <map>

void print_bar_chart(const std::map<std::string, int> &data,
					 const std::string &chart_title,
					 const std::string &x_axis_title,
					 const std::string &y_axis_title,
					 int bar_width,
					 int bar_gap,
					 const std::string &output_filename)
{

	// Find the maximum value in the data
	int max_value = 0;
	for (const auto &[key, value] : data)
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
	int chart_width = (bar_width + bar_gap) * data.size() + bar_gap + 100;

	// Create the SVG document
	std::stringstream svg;
	svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << chart_width << "\" height=\"" << chart_height << "\" viewBox=\"0 0 " << chart_width << " " << chart_height << "\">\n";

	// Create the chart title
	svg << "<text x=\"" << chart_width / 2 << "\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << chart_title << "</text>\n";

	// Create the x-axis title
	svg << "<text x=\"" << chart_width / 2 << "\" y=\"" << chart_height - 10 << "\" text-anchor=\"middle\" font-size=\"20\">" << x_axis_title << "</text>\n";

	// Create the y-axis title
	svg << "<text x=\"10\" y=\"" << chart_height / 2 + 10 << "\" text-anchor=\"middle\" font-size=\"20\" transform=\"rotate(270, 10, " << chart_height / 2 << ")\">" << y_axis_title << "</text>\n";

	// Create the bars
	int x = bar_gap + 50;
	for (const auto &[key, value] : data)
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

void print_line_chart(const std::map<std::string, int> &data,
					  const std::string &chart_title,
					  const std::string &x_axis_title,
					  const std::string &y_axis_title,
					  int line_width,
					  const std::string &output_filename)
{
	// Find the maximum value in the data
	int max_value = 0;
	for (const auto &[key, value] : data)
	{
		if (value > max_value)
		{
			max_value = value;
		}
	}

	// idkman
}

void print_pie_chart(const std::map<std::string, int> &data,
					 const std::string &chart_title,
					 const std::string &output_filename)
{

	// Calculate the value into percentages
	int total = 0;
	for (const auto &[key, value] : data)
	{
		total += value;
	}

	// Create the SVG document
	std::stringstream svg;
	svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"500\" height=\"500\" viewBox=\"0 0 500 500\">\n";

	// Create the chart title
	svg << "<text x=\"250\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << chart_title << "</text>\n";

	// Create the circle
	svg << "<circle cx=\"250\" cy=\"250\" r=\"200\" fill=\"blue\" stroke=\"black\" stroke-width=\"2\" />\n";

	// Create the pie chart
	int start_angle = 0;
	int remaining_angle = 360;
	auto it = data.begin();
	while (it != data.end())
	{
		int angle = it->second * 360 / total;
		if (std::next(it) == data.end())
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

void histogram(const std::map<std::string, int> &data)
{
	auto it = data.begin();
	while (it != data.end())
	{
		std::cout << it->first << ": ";
		for (int i = 0; i < it->second; i++)
		{
			std::cout << "*";
		}

		std::cout << it->second << std::endl;
		it++;
	}
}

int main(int argc, const char *const *const argv)
try
{

#ifndef NDEBUG
	const std::string settings_filename = "../settings.json";
	const std::string data_filename = "../data.csv";
#else
	const std::string settings_filename = "settings.json";
	const std::string data_filename = "data.csv";
#endif

	JsonReader reader;
	CSVReader csvReader;

	// TOOD: Make readFile both consistent
	reader.readFile(settings_filename);
	const auto &csv_data = csvReader.readFile(data_filename);

	/*
	std::cout << "Chart Type: " << data[0] << std::endl;
	std::cout << "Chart Title: " << data[1] << std::endl;
	std::cout << "X Axis Title: " << data[2] << std::endl;
	std::cout << "Y Axis Title: " << data[3] << std::endl;
	std::cout << "Axis Anchor: " << data[4] << std::endl;
	*/
	const auto &data = reader.getData();
	Chart chart = Chart(
		Chart::typeFromString(data[0]), data[1], data[2], data[3], data[4]);

	switch (chart.getType())
	{
	case Chart::Type::BAR:
		std::cout << "Bar chart" << std::endl;
		print_bar_chart(csv_data, data[1], data[2], data[3], 50, 10, "output.svg");
		break;
	case Chart::Type::LINE:
		std::cout << "Line chart" << std::endl;
		print_line_chart(csv_data, data[1], data[2], data[3], 10, "output.svg");
		break;
	case Chart::Type::PIE:
		std::cout << "Pie chart" << std::endl;
		print_pie_chart(csv_data, data[1], "output.svg");
		break;
	default:
		std::cout << "Invalid chart type" << std::endl;
		break;
	}

	histogram(csv_data);

	/*
	const auto& csv_data = Csv_Reader("data.csv");
	for (const auto& [key, value] : csv_data) {
		std::cout << key << " " << value << std::endl;
	}
	*/

	return 0;
}
catch (const std::exception &e)
{
	std::cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...)
{
	std::cerr << "Unknown exception\n";
	return 2;
}