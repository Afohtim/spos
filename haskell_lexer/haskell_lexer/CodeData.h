#pragma once
#include <string>

namespace lexer
{
	struct Code_data
	{
		std::string current_line;
		int line;
		int position;
		bool broken_token = false;
	};
}


