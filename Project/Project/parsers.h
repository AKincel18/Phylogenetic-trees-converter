#pragma once

template <typename Iterator>
struct newick_grammar_parser : boost::spirit::qi::grammar<Iterator, phylo_tree(), boost::spirit::ascii::space_type>
{
public:
	newick_grammar_parser() : newick_grammar_parser::base_type(tree)
	{

		tree %= subtree >> ';';

		subtree =
			-children[boost::phoenix::at_c<0>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >>

			(
				(only_branch_length[boost::phoenix::at_c<2>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >> 
				!boost::spirit::qi::lit('_') )
				|
				(
					-name[boost::phoenix::at_c<1>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >>
					( (branch_length[boost::phoenix::at_c<2>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >>
						-('[' >> boost::spirit::qi::int_ >> ']' )
						)
						|
						empty[boost::phoenix::at_c<2>(boost::spirit::qi::_val) = 0.0])
					)
				);
					
		children =
			'(' >> subtree[boost::phoenix::push_back(boost::spirit::qi::_val, boost::spirit::qi::_1)] >>
			*(',' >> subtree[boost::phoenix::push_back(boost::spirit::qi::_val, boost::spirit::qi::_1)]) >> ')';

		name %= boost::spirit::qi::lexeme[*(boost::spirit::qi::char_('!', '~') - ':' - ')' - ',' - ';')];


		branch_length %= ':' >> boost::spirit::qi::double_;

		only_branch_length %=  boost::spirit::qi::double_;

		empty = "";
	}

private:

	boost::spirit::qi::rule<Iterator, phylo_tree(), boost::spirit::ascii::space_type> tree, subtree;
	boost::spirit::qi::rule<Iterator, std::vector<phylo_tree>(), boost::spirit::ascii::space_type> children;
	boost::spirit::qi::rule<Iterator, double()> branch_length, only_branch_length;
	boost::spirit::qi::rule<Iterator, std::string()> name, empty;
};

template <typename Iterator>
struct phyloxml_grammar_parser : boost::spirit::qi::grammar<Iterator, phylo_tree(), boost::spirit::qi::locals<std::string>, boost::spirit::ascii::space_type>
{
	phyloxml_grammar_parser() : phyloxml_grammar_parser::base_type(tree)
	{

		tree %= boost::spirit::lit("<phyloxml>") >> boost::spirit::lit("<phylogeny>") >> subtree >>
			boost::spirit::lit("</phylogeny>") >> boost::spirit::lit("</phyloxml>");

		subtree =
			"<clade>" >>
			(("<name>" >> name[boost::phoenix::at_c<1>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >> "</name>") 
				|
				empty[boost::phoenix::at_c<1>(boost::spirit::qi::_val) = ""]) >>

			( ("<branch_length>" >> branch_length[boost::phoenix::at_c<2>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >> "</branch_length>")
			| empty[boost::phoenix::at_c<2>(boost::spirit::qi::_val) = 0.0] ) >>	
			-children[boost::phoenix::at_c<0>(boost::spirit::qi::_val) = boost::spirit::qi::_1] >> 
			
			"</clade>";

		children = *((&boost::spirit::ascii::string("<clade>") >> subtree[boost::phoenix::push_back(boost::spirit::qi::_val, boost::spirit::qi::_1)]) | unknown_node);
		branch_length %= boost::spirit::qi::double_;

		name %= boost::spirit::qi::lexeme[*(boost::spirit::qi::char_ - '<')];

		unknown_node =
			start_tag[boost::spirit::qi::_val = boost::spirit::qi::_1]
			>> *(unknown_node | text)
			>> end_tag(boost::spirit::qi::_val);

		start_tag = '<' >> !boost::spirit::lit('/')
			>> boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - ' ' - '>')[boost::spirit::qi::_val += boost::spirit::qi::_1]]
			>> -(' ' >> -attributes) >> ">";


		end_tag = "</" >> boost::spirit::lit(boost::spirit::qi::labels::_r1) >> '>';

		attributes = (attribute >> *(' ' >> attribute));

		attribute = boost::spirit::qi::lexeme[*(boost::spirit::qi::char_ - '=')] >> '=' >> attribute_value;

		attribute_value = '"' >> *(boost::spirit::qi::char_ - boost::spirit::qi::char_('"')) >> '"';

		text = boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - '<')];

		empty = "";

	}

private:
	boost::spirit::qi::rule<Iterator, phylo_tree(), boost::spirit::qi::locals<std::string>, boost::spirit::ascii::space_type> tree, subtree;
	boost::spirit::qi::rule<Iterator, std::vector<phylo_tree>(), boost::spirit::ascii::space_type> children;
	boost::spirit::qi::rule<Iterator, double()> branch_length;
	boost::spirit::qi::rule<Iterator, std::string()> name, unknown_node, start_tag, text, empty, attribute, attribute_value, attributes;
	boost::spirit::qi::rule<Iterator, void(std::string)> end_tag;

};