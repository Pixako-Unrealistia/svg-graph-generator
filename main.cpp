#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <map>

class Json_Reader {
public:
    Json_Reader() {}


	//reader function
    bool ReadFile(const std::string& filename) {
        std::ifstream json_file(filename);
		
		//check if file valid?

        if (!json_file.is_open()) {
            std::cout << "Error opening file. The name must be 'settings.json' and in the same folder as main.cpp!" << std::endl;
            return false;
        }

        std::stringstream buffer;
        buffer << json_file.rdbuf();
        json_string_ = buffer.str();

        return true;
    }

    std::vector<std::string> GetData() const {
        std::vector<std::string> json_data;

        size_t pos = json_string_.find("Chart_Type");
        std::string chart_type = GetValue(pos, "Chart_Type");
        json_data.push_back(chart_type);

        pos = json_string_.find("Chart_Title");
        std::string chart_title = GetValue(pos, "Chart_Title");
        json_data.push_back(chart_title);

        pos = json_string_.find("X_Axis_Title");
        std::string x_axis_title = GetValue(pos, "X_Axis_Title");
        json_data.push_back(x_axis_title);

        pos = json_string_.find("Y_Axis_Title");
        std::string y_axis_title = GetValue(pos, "Y_Axis_Title");
        json_data.push_back(y_axis_title);

        pos = json_string_.find("Axis_Anchor");
        std::string axis_anchor = GetValue(pos, "Axis_Anchor");
        json_data.push_back(axis_anchor);

        return json_data;
    }

private:

	std::string GetValue(size_t pos, const std::string& key) const {
		const int OFFSET = key.size() + 3;
		std::string value = json_string_.substr(pos + OFFSET, json_string_.find(",", pos) - (pos + OFFSET));
		// replace all quotes with empty strings
		size_t quote_pos = value.find("\"");
		while (quote_pos != std::string::npos) {
			value.replace(quote_pos, 1, "");
			quote_pos = value.find("\"");
		}
		//remove all spaces
		size_t space_pos = value.find(" ");
		while (space_pos != std::string::npos) {
			value.replace(space_pos, 1, "");
			space_pos = value.find(" ");
		}


		return value;
	}
	

    std::string json_string_;
};



std::map<std::string, int> Csv_Reader(const std::string& file_name) {
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return {};
    }

    std::map<std::string, int> data;

    std::string line;
    while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string key;
		std::string value;
		std::getline(ss, key, ',');
		std::getline(ss, value, ',');
		data[key] = std::stoi(value);
    }

    file.close();
    return data;
}

void print_bar_chart(const std::map<std::string, int>& data,
                   const std::string& chart_title,
                   const std::string& x_axis_title,
                   const std::string& y_axis_title,
                   int bar_width,
                   int bar_gap,
                   const std::string& output_filename) {

    // Find the maximum value in the data
    int max_value = 0;
    for (const auto& [key, value] : data) {
		//std::cout << key << " " << value << std::endl;
		//std::cout << "max_value: " << max_value << std::endl;
        if (value > max_value) {
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
	svg << "<text x=\"10\" y=\"" << chart_height / 2 + 10<< "\" text-anchor=\"middle\" font-size=\"20\" transform=\"rotate(270, 10, " << chart_height / 2 << ")\">" << y_axis_title << "</text>\n";

	// Create the bars
	int x = bar_gap + 50;
	for (const auto& [key, value] : data) {
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
    if (output_file.is_open()) {
        output_file << svg.str();
        output_file.close();
    } else {
        std::cerr << "Error: could not write to file " << output_filename << std::endl;
    }
}

void print_line_chart(const std::map<std::string, int>& data,
                   const std::string& chart_title,
                   const std::string& x_axis_title,
                   const std::string& y_axis_title,
				   int line_width,
                   const std::string& output_filename)
{
	// Find the maximum value in the data
	int max_value = 0;
	for (const auto& [key, value] : data) {
		if (value > max_value) {
			max_value = value;
		}
	}
	
	//idkman

}

void print_pie_chart(const std::map<std::string, int>& data,
	const std::string& chart_title,
	const std::string& output_filename)
{

	// Calculate the value into percentages
	int total = 0;
	for (const auto& [key, value] : data) {
		total += value;
	}

	// Create the SVG document
	std::stringstream svg;
	svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"500\" height=\"500\" viewBox=\"0 0 500 500\">\n";
	
	// Create the chart title
	svg << "<text x=\"250\" y=\"30\" text-anchor=\"middle\" font-size=\"20\">" << chart_title << "</text>\n";

	// Create the circle
	svg << "<circle cx=\"250\" cy=\"250\" r=\"200\" fill=\"white\" stroke=\"black\" stroke-width=\"2\" />\n";

	// Create the pie chart
	int start_angle = 0;
	int remaining_angle = 360;
	auto it = data.begin();
	while (it != data.end()) {
		int angle = it->second * 360 / total;
		if (std::next(it) == data.end()) {
			angle = remaining_angle;
		}
		int end_angle = start_angle + angle;
		int x1 = 250 + 200 * std::cos(start_angle * 3.14159 / 180);
		int y1 = 250 + 200 * std::sin(start_angle * 3.14159 / 180);
		int x2 = 250 + 200 * std::cos(end_angle * 3.14159 / 180);
		int y2 = 250 + 200 * std::sin(end_angle * 3.14159 / 180);
		svg << "<path d=\"M250,250 L" << x1 << "," << y1 << " A200,200 0 " << (angle > 180 ? 1 : 0) << ",1 " << x2 << "," << y2 << " Z\" fill=\"blue\" />\n";
		//write the text
		int x3 = 250 + 200 * std::cos((start_angle + angle / 2) * 3.14159 / 180);
		int y3 = 250 + 200 * std::sin((start_angle + angle / 2) * 3.14159 / 180);
		svg << "<text x=\"" << x3 << "\" y=\"" << y3 << "\" text-anchor=\"middle\" font-size=\"20\">" << it->first << "</text>\n";
		start_angle = end_angle;
		remaining_angle -= angle;
		it++;
	}

	// Write the SVG document to a file
	std::ofstream output_file(output_filename);
	if (output_file.is_open()) {
		output_file << svg.str();
		output_file.close();
	}
	else {
		std::cerr << "Error: could not write to file " << output_filename << std::endl;
	}
}


int main() {
    Json_Reader reader;
    if (!reader.ReadFile("settings.json")) {
        return 1;
    }

    const auto& data = reader.GetData();
	/*
    std::cout << "Chart Type: " << data[0] << std::endl;
    std::cout << "Chart Title: " << data[1] << std::endl;
    std::cout << "X Axis Title: " << data[2] << std::endl;
    std::cout << "Y Axis Title: " << data[3] << std::endl;
    std::cout << "Axis Anchor: " << data[4] << std::endl;
	*/

	std::string chart_type = data[0];
	std::string chart_title = data[1];
	std::string x_axis_title = data[2];
	std::string y_axis_title = data[3];
	std::string axis_anchor = data[4];




	std::cout << "Chart Type:" << chart_type << std::endl;
	if (data[0] == "bar") {
		std::cout << "Bar chart" << std::endl;
		const auto& csv_data = Csv_Reader("data.csv");
		print_bar_chart(csv_data, data[1], data[2], data[3], 50, 10, "output.svg");
	}
	else if (data[0] == "line") {
		std::cout << "Line chart" << std::endl;
		const auto& csv_data = Csv_Reader("data.csv");
		print_line_chart(csv_data, data[1], data[2], data[3], 10, "output.svg");
	}
	else if (data[0] == "pie") {
		std::cout << "Pie chart" << std::endl;
		const auto& csv_data = Csv_Reader("data.csv");
		print_pie_chart(csv_data, data[1], "output.svg");
	}
	else {
		std::cout << "Invalid chart type" << std::endl;
	}



	/*
	const auto& csv_data = Csv_Reader("data.csv");
	for (const auto& [key, value] : csv_data) {
		std::cout << key << " " << value << std::endl;
	}*/


    return 0;
}