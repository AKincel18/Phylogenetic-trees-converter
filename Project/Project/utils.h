#pragma once

void fetch_file_content(const std::string& file_name, std::pair<bool, std::string&>& result_file);
std::tuple<bool, std::string, std::string> get_files(int argc, char* argv[]);
void save_output(const std::string& file_name, const std::string& output);