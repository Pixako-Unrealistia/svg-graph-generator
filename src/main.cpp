
#include "JsonReader.hpp"
#include "CSVReader.hpp"
#include "Chart.hpp"

#include <iostream>
#include <string>
#include <map>

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

	// TODO: Make readFile both consistent
	reader.readFile(settings_filename);
	const std::map<std::string, int> &csv_data = csvReader.readFile(data_filename);

	/*
	std::cout << "Chart Type: " << data[0] << std::endl;
	std::cout << "Chart Title: " << data[1] << std::endl;
	std::cout << "X Axis Title: " << data[2] << std::endl;
	std::cout << "Y Axis Title: " << data[3] << std::endl;
	std::cout << "Axis Anchor: " << data[4] << std::endl;
	*/
	const auto &data = reader.getData();
	Chart chart = Chart(
		Chart::typeFromString(data[0]), data[1], data[2], data[3], data[4], csv_data);

	switch (chart.getType())
	{
	case Chart::Type::BAR:
		std::cout << "Bar chart" << std::endl;
		chart.printBarChart(50, 10, "output.svg");
		break;
	case Chart::Type::LINE:
		std::cout << "Line chart" << std::endl;
		// TODO
		// print_line_chart(csv_data, data[1], data[2], data[3], 10, "output.svg");
		break;
	case Chart::Type::PIE:
		std::cout << "Pie chart" << std::endl;
		chart.printPieChart("output.svg");
		break;
	}

	chart.printHistogramChart();

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