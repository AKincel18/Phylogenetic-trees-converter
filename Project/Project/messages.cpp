#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include "messages.h"

const boost::unordered_map<messages, const char*> init_messages() {
	return boost::assign::map_list_of
		(messages::SAVED, "Successfully completed! Saved to file: ")
		(messages::PARSING_ERROR, "Error! Failed to parse input file.")
		(messages::GENERATING_ERROR, "Error! Failed to generate output file.")
		(messages::WRONG_ARGUMENTS, "Error! Wrong number of arguments.")
		(messages::OPENING_FILE_ERROR, "Error! Could not open input file: ");
}