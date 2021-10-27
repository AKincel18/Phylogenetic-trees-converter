#pragma once

enum class messages { 
	SAVED, 
	PARSING_ERROR, 
	GENERATING_ERROR,
	WRONG_ARGUMENTS, 
	OPENING_FILE_ERROR 
};

const boost::unordered_map<messages, const char*> init_messages();