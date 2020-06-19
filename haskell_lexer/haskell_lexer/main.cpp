#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"


int main()
{
	std::vector<lexer::Token> tokens;
	std::vector<std::string> symbol_table;
	lexer::lex("code.hs", tokens, symbol_table);

	for (lexer::Token token : tokens)
	{
		std::cout << lexer::Token_to_string[(int)token.type];
		if (token.symbol_table_id != RAND_MAX)
		{
			if (token.type != lexer::Token_type::Whitechar)
				std::cout << ' ' << symbol_table[token.symbol_table_id] << ' ';
			else
			{
				if (symbol_table[token.symbol_table_id] == "\n")
					std::cout << " \\n ";
				else if (symbol_table[token.symbol_table_id] == "\t")
					std::cout << " \\t ";
				else if (symbol_table[token.symbol_table_id] == " ")
					std::cout << " space ";

			}
		}
		std::cout << std::endl;
	}
	std::cin.get();

}