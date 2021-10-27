#include <string>
#include "tab_printer.h"

std::string print_tabs(int tab_counter) {
	std::string tabs;
	for (int i = 0; i < tab_counter; i++) {
		tabs.append("\t");
	}
	return tabs;
}