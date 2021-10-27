#pragma once

struct phylo_tree
{
	std::vector<phylo_tree> children;
	std::string name;
	double length;

	std::string to_string() {
		return name + " (" + std::to_string(length) + ")";
	}
};