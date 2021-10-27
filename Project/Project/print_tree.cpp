#include <string>
#include <vector>
#include <iomanip>
#include "phylo_tree.h"
#include "print_tree.h"

void print_tree(std::ostream& stream, phylo_tree& tree, int level)
{
	if (level == 1)
	{
		stream << tree.to_string() << '\n';
		level++;
	}
	for (std::vector<phylo_tree>::const_iterator it = tree.children.begin(); it != tree.children.end(); it++)
	{
		phylo_tree children = *it;
		stream << std::setw(level * 10) << children.to_string() << std::endl;
		print_tree(stream, children, level + 1);
	}
}

std::ostream& operator<<(std::ostream& stream, phylo_tree& tree)
{
	print_tree(stream, tree, 1);
	return stream;
}