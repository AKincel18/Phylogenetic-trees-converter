#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/unordered/unordered_map.hpp>
#include "Project.h"
#include "phylo_tree.h"
#include "utils.h"
#include "print_tree.h"
#include "messages.h"
#include "parsers.h"
#include "generators.h"

BOOST_FUSION_ADAPT_STRUCT(
	phylo_tree,
	(std::vector<phylo_tree>, children)
	(std::string, name)
	(double, length)
)

template <typename Parser, typename Generator>
messages parse_and_generate(std::string& input_file_content, phylo_tree& tree, std::back_insert_iterator<std::string>& it_output) {
	Parser parser;
	Generator generator;
	if (boost::spirit::qi::phrase_parse(input_file_content.cbegin(), input_file_content.cend(), parser, boost::spirit::ascii::space, tree)) {
		return boost::spirit::karma::generate(it_output, generator, tree) ? messages::SAVED : messages::GENERATING_ERROR;
	}
	else {
		return messages::PARSING_ERROR;
	}
}

int main(int argc, char* argv[])
{
	const boost::unordered_map<messages, const char*> msg = init_messages();
	std::tuple<bool, std::string, std::string> files = get_files(argc, argv);
	if (!std::get<0>(files)) {
		std::cout << msg.at(messages::WRONG_ARGUMENTS) << std::endl;
		return 1;
	}
	std::string input_file_name = std::get<1>(files);
	std::string output_file_name = std::get<2>(files);


	std::string input_file_content;
	std::pair<bool, std::string&> result_file = {false, std::ref(input_file_content)};
	fetch_file_content(input_file_name, result_file);
	if (!result_file.first) {
		std::cout << msg.at(messages::OPENING_FILE_ERROR) << input_file_name << std::endl;
		return 1;
	}
	input_file_content = result_file.second;

	phylo_tree tree;
	std::string output_file_content;
	std::back_insert_iterator<std::string> it_output(output_file_content);

	messages result = parse_and_generate<
			newick_grammar_parser<std::string::const_iterator>,
			phyloxml_grammar_generator<std::back_insert_iterator<std::string>>>(input_file_content, tree, it_output);
	if (result != messages::SAVED) {
		result = parse_and_generate<
			phyloxml_grammar_parser<std::string::const_iterator>,
			newick_grammar_generator<std::back_insert_iterator<std::string>>>(input_file_content, tree, it_output);
	}	

	if (result == messages::SAVED) {
		//std::cout << tree << std::endl;
		std::cout << msg.at(result) << output_file_name << std::endl;
		save_output(output_file_name, output_file_content);
	}
	else {
		std::cout << msg.at(result) << std::endl;
	}

	return 0;
}
