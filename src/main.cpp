
#include "OrderedMap.hpp"
#include "JsonReader.hpp"
#include "CSVReader.hpp"
#include "Chart.hpp"

#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, const char *const *const argv)
try
{

// NDEBUG is defined when compiling in debug mode using CMake
#ifndef NDEBUG
	const std::string settings_filename = "../settings.json";
	const std::string data_filename = "../data.csv";
#else
	const std::string settings_filename = "settings.json";
	const std::string data_filename = "data.csv";
#endif

	JsonReader jsonReader;
	CSVReader csvReader;

	jsonReader.readFile(settings_filename);
	csvReader.readFile(data_filename);

	/*
	std::cout << "Chart Type: " << data[0] << std::endl;
	std::cout << "Chart Title: " << data[1] << std::endl;
	std::cout << "X Axis Title: " << data[2] << std::endl;
	std::cout << "Y Axis Title: " << data[3] << std::endl;
	std::cout << "Axis Anchor: " << data[4] << std::endl;
	*/

	const std::vector<std::string> &settings_data = jsonReader.getData();
	const OrderedMap<std::string, double> &csv_data = csvReader.getData();

	Chart chart = Chart(
		Chart::typeFromString(settings_data[0]), settings_data[1], settings_data[2], settings_data[3], settings_data[4], csv_data);

	switch (chart.getType())
	{
	case Chart::Type::BAR:
		chart.printBarChart(50, 10, "output.svg");
		break;
	case Chart::Type::LINE:
		chart.printLineChart(10, "output.svg");
		break;
	case Chart::Type::PIE:
		chart.printPieChart("output.svg");
		break;
	case Chart::Type::SCATTER:
		chart.printScatterChart(10, "output.svg");
		break;
	}

	std::string absolute_path = std::filesystem::absolute("output.svg").string();
	std::cout << chart.getTypeString() << " chart exported to " << absolute_path << std::endl;

	std::cout << "Histogram chart" << std::endl;
	chart.printHistogramChart();
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