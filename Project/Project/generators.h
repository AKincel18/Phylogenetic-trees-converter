#pragma once
#include "tab_printer.h"

template <typename Iterator>
struct newick_grammar_generator : boost::spirit::karma::grammar<Iterator, phylo_tree()> {
	newick_grammar_generator() :newick_grammar_generator::base_type(tree) {
		
		tree = subtree << ";";
		subtree =
			-children <<
			name <<
			branch_length(boost::phoenix::at_c<1>(boost::spirit::karma::_val), boost::phoenix::at_c<2>(boost::spirit::karma::_val));

		children = first << *next << ')';
		first = '(' << subtree;
		next = ',' << subtree;
		name = *boost::spirit::karma::char_;
		branch_length =
			boost::spirit::karma::eps(boost::spirit::karma::_r2 == 0.0) |
			(boost::spirit::karma::eps(boost::spirit::karma::_r1 == "") << boost::spirit::karma::double_) |
			(":" << boost::spirit::karma::double_);

	}

private:

	boost::spirit::karma::rule<Iterator, phylo_tree()> tree, subtree, first, next;
	boost::spirit::karma::rule<Iterator, std::vector<phylo_tree>()> children;
	boost::spirit::karma::rule<Iterator, double(std::string, double)> branch_length;
	boost::spirit::karma::rule<Iterator, std::string()> name;
};


template <typename Iterator>
struct phyloxml_grammar_generator : boost::spirit::karma::grammar<Iterator, phylo_tree()> {
	phyloxml_grammar_generator() :phyloxml_grammar_generator::base_type(tree) {
		
		auto _tab_counter = boost::phoenix::ref(tab_counter);
		tree = "<phyloxml>\n\t<phylogeny>\n" << subtree << "\t</phylogeny>\n</phyloxml>";

		subtree =
			boost::spirit::karma::lit(boost::phoenix::bind(&print_tabs, _tab_counter)) <<
			"<clade>" << 
			boost::spirit::karma::eps[_tab_counter++] <<
			name(boost::phoenix::at_c<1>(boost::spirit::karma::_val))[boost::spirit::karma::_1 = boost::phoenix::at_c<1>(boost::spirit::karma::_val)] <<
			branch_length(boost::phoenix::at_c<2>(boost::spirit::karma::_val))[boost::spirit::karma::_1 = boost::phoenix::at_c<2>(boost::spirit::karma::_val)] <<
			"\n" << 
			
			-children[boost::spirit::karma::_1 = boost::phoenix::at_c<0>(boost::spirit::karma::_val)] <<
			boost::spirit::karma::eps[_tab_counter--]
			<< boost::spirit::karma::lit(boost::phoenix::bind(&print_tabs, _tab_counter)) <<
			"</clade>\n";

		children = *subtree;

		branch_length = boost::spirit::karma::eps(boost::spirit::karma::_r1 == 0.0) 
			|
					("\n" << boost::spirit::karma::lit(boost::phoenix::bind(&print_tabs, _tab_counter)) <<
					"<branch_length>" << boost::spirit::karma::double_(boost::spirit::karma::_r1) << "</branch_length>");

		name = boost::spirit::karma::eps(boost::spirit::karma::_r1 == "") 
				| 
					("\n" << boost::spirit::karma::lit(boost::phoenix::bind(&print_tabs, _tab_counter)) << 
					"<name>" << boost::spirit::karma::lit(boost::spirit::karma::_r1) << "</name>");
	}

private:
	int tab_counter = 2;
	boost::spirit::karma::rule<Iterator, phylo_tree()> tree, subtree;
	boost::spirit::karma::rule<Iterator, std::vector<phylo_tree>()> children;
	boost::spirit::karma::rule<Iterator, double(double)> branch_length;
	boost::spirit::karma::rule<Iterator, std::string(std::string)> name;

};