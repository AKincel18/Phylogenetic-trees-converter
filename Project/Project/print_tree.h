#pragma once

void print_tree(std::ostream& stream, phylo_tree& tree, int level);
std::ostream& operator<<(std::ostream& stream, phylo_tree& tree);