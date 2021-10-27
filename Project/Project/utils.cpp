#include <boost/spirit/include/qi.hpp>
#include <fstream>
#include <map>
#include "utils.h"

void fetch_file_content(const std::string& file_name, std::pair<bool, std::string&>& result_file) {
	std::ifstream in_file(file_name, std::ios_base::in);
	if (in_file)
	{
		in_file.unsetf(std::ios::skipws);
		std::copy(
			std::istream_iterator<char>(in_file),
			std::istream_iterator<char>(),
			std::back_inserter(result_file.second));
		in_file.close();
		result_file.first = true;
	}
}

std::tuple<bool, std::string, std::string> get_files(int argc, char* argv[]) {
	std::string input, output;
	if (argc != 5)
	{
		return { false, input, output };
	}
	for (int i = 0; i < argc - 1; i++)
	{
		if (strcmp(argv[i], "-i") == 0)
		{
			input = argv[i + 1];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			output = argv[i + 1];
		}
	}
	return { true, input, output };
}

void save_output(const std::string& file_name, const std::string& output) {
	std::ofstream file;
	file.open(file_name);
	file << output;
	file.close();
}