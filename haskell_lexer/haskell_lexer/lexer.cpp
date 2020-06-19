#include "lexer.h"

#include <fstream>
#include <vector>
#include <cctype>

namespace lexer {

	int ncomment_count = 0;
	std::string comment_data;

	bool is_digit(char c)
	{
		return std::isdigit(c);
	}

	bool is_octit(char c)
	{
		return std::isdigit(c) && c < '8';
	}

	bool is_hexit(char c)
	{
		return std::isxdigit(c);
	}

	bool is_symbol(char c)
	{
		switch (c)
		{
		case '!':
		case '#':
		case '$':
		case '%':
		case '&':
		case '*':
		case '+':
		case '.':
		case '/':
		case '<':
		case '=':
		case '>':
		case '?':
		case '@':
		case '\\':
		case '^':
		case '|':
		case '-':
		case '~':
			return true;
		default:
			return false;
		}
	}

	bool is_large(char c)
	{
		return std::isupper(c);
	}

	bool is_small(char c)
	{
		return std::islower(c) || c == '_';
	}

	bool is_graphic(char c)
	{
		return is_small(c) || is_large(c) || is_symbol(c) || is_digit(c) || is_special(c) || c == ':' || c == '"' || c == '\'';
	}

	bool is_any(char c)
	{
		return is_graphic(c) || c == ' ' || c == '\t';
	}

	bool is_whitechar(char c)
	{
		return std::isspace(c);
	}

	bool is_special(char c)
	{
		switch (c) {
			case '(':
			case ')':
			case ',':
			case ';':
			case '[':
			case ']':
			case '`':
			case '{':
			case '}':
				return true;
			default:
				return false;
		}
	}

	bool is_ANY(char c)
	{
		return is_graphic(c) || is_whitechar(c);
	}


	int add_to_symbol_table(std::vector<std::string> &symbol_table, std::string content)
	{
		symbol_table.push_back(content);
		return symbol_table.size() - 1;
	}

	void token_error(Code_data &code_data, Token &token, std::vector<Token> &tokens)
	{
		code_data.broken_token = true;
		token.type = Token_type::Token_error;
		tokens.push_back(token);
	}


	void handle_comment(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Comment, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";
		while (code_data.current_line.size() > code_data.position && code_data.current_line[code_data.position] == '-') 
		{
			content.push_back('-');
			code_data.position++;
		}

		bool first = true;
		while (code_data.current_line.size() > code_data.position)
		{
			if (first)
			{
				first = false;
				if (is_symbol(code_data.current_line[code_data.position]))
				{
					token_error(code_data, token, tokens);
					return;
				}
			}
				if (is_any(code_data.current_line[code_data.position]))
				{
					content.push_back(code_data.current_line[code_data.position]);
				}
				else
				{
					//token error
				}

			code_data.position++;
		}


		int id = add_to_symbol_table(symbolTable, content);
		token.symbol_table_id = id;
		tokens.push_back(token);
	}

	void handle_ncomment(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable, std::string previous_content)
	{
		Token token = Token(Token_type::Ncomment, code_data.line, code_data.position, RAND_MAX);
		std::string content = previous_content;
		while (code_data.current_line.size() > code_data.position +1 && code_data.current_line[code_data.position] == '{' && code_data.current_line[code_data.position] == '-')
		{
			content.push_back('{');
			content.push_back('-');
			code_data.position += 2;
		}

		bool first = true;
		while (code_data.current_line.size() > code_data.position)
		{
			
			if (is_ANY(code_data.current_line[code_data.position]))
			{
				content.push_back(code_data.current_line[code_data.position]);
			}
			else
			{
				if (code_data.current_line.size() > code_data.position + 1 && code_data.current_line[code_data.position] == '{' && code_data.current_line[code_data.position] == '-')
				{
					content.push_back('{');
					content.push_back('-');
					code_data.position += 2;
					ncomment_count++;
				}

				if (code_data.current_line[code_data.position] == '}' && content.size() > 2 && content.back() == '-')
				{
					content.push_back('}');
					ncomment_count--;
					if (!ncomment_count)
					{
						int id = add_to_symbol_table(symbolTable, content);
						token.symbol_table_id = id;
						tokens.push_back(token);
						return;
					}
					
				}
				else
				{
					// token error
				}
			}
			code_data.position++;
		}

		if (!ncomment_count)
		{
			int id = add_to_symbol_table(symbolTable, content);
			token.symbol_table_id = id;
			tokens.push_back(token);
		}
		else
		{
			comment_data = content;
		}

		
	}

	void handle_ncomment(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		handle_ncomment(code_data, tokens, symbolTable, "");
	}

	void handle_special(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Special_begin, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		if (code_data.current_line.size() > code_data.position && is_special(code_data.current_line[code_data.position]))
		{
			switch (code_data.current_line[code_data.position])
			{
			case '(':
				token.type = Token_type::Parenthese_left;
				break;
			case ')':
				token.type = Token_type::Parenthese_right;
				break;
			case ',':
				token.type = Token_type::Comma;
				break;
			case ';':
				token.type = Token_type::Semicolon;
				break;
			case '[':
				token.type = Token_type::Bracket_left;
				break;
			case ']':
				token.type = Token_type::Bracket_right;
				break;
			case '`':
				token.type = Token_type::Backtick;
				break;
			case '{':
				token.type = Token_type::Brace_left;
				break;
			case '}':
				token.type = Token_type::Brace_right;
				break;
			}
			code_data.position++;
		}
		tokens.push_back(token);
	}

	void try_handle_reservedid(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable, std::string &content, Token &token)
	{
		if (content == "case")
		{
			token.type = Token_type::Case;
		}
		else if (content == "class")
		{
			token.type = Token_type::Class;

		}
		else if (content == "data")
		{
			token.type = Token_type::Data;

		}
		else if (content == "default")
		{
			token.type = Token_type::Default;

		}
		else if (content == "deriving")
		{
			token.type = Token_type::Deriving;

		}
		else if (content == "do")
		{
			token.type = Token_type::Do;

		}
		else if (content == "else")
		{
			token.type = Token_type::Else;

		}
		else if (content == "if")
		{
			token.type = Token_type::If;

		}
		else if (content == "import")
		{
			token.type = Token_type::Import;

		}
		else if (content == "in")
		{
			token.type = Token_type::In;

		}
		else if (content == "infix")
		{
			token.type = Token_type::Infix;

		}
		else if (content == "infixl")
		{
			token.type = Token_type::Infixl;

		}
		else if (content == "infixr")
		{
			token.type = Token_type::Infixr;

		}
		else if (content == "instance")
		{
			token.type = Token_type::Instance;

		}
		else if (content == "let")
		{
			token.type = Token_type::Let;

		}
		else if (content == "module")
		{
			token.type = Token_type::Module;

		}
		else if (content == "newtype")
		{
			token.type = Token_type::Newtype;

		}
		else if (content == "of")
		{
			token.type = Token_type::Of;

		}
		else if (content == "then")
		{
			token.type = Token_type::Then;

		}
		else if (content == "type")
		{
			token.type = Token_type::Type;

		}
		else if (content == "where")
		{
			token.type = Token_type::Where;

		}
		else if (content == "_")
		{
			token.type = Token_type::Underscore;

		}

	}


	void handle_qvarid(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Qvarid, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		bool first = true;
		while (is_small(c) || is_large(c) || is_digit(c) || c == '\'')
		{
			if (first)
			{
				first = false;
				if (!is_small(c))
				{
					token_error(code_data, token, tokens);
					return;
				}
			}

			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
				break;
		}
		try_handle_reservedid(code_data, tokens, symbolTable, content, token);
		if (!(token.type < Token_type::Reservedid_end && token.type > Token_type::Reservedop_begin))
		{
			int id = add_to_symbol_table(symbolTable, content);
			token.symbol_table_id = id;
		}
			
		tokens.push_back(token);
		
	}


	void handle_qconid(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Qconid, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		bool first = true;
		while (is_small(c) || is_large(c) || is_digit(c) || c == '\'')
		{
			if (first)
			{
				first = false;
				if (!is_large(c))
				{
					token_error(code_data, token, tokens);
					return;
				}
			}

			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
				break;
		}
		int id = add_to_symbol_table(symbolTable, content);
		token.symbol_table_id = id;
		tokens.push_back(token);
	}

	bool is_final_reservedop_dfa_state(int state)
	{
		return state == 2 || state == 3 || state == 4 || state == 5 || state == 7 || state == 9 || state == 10 || state == 11 || state == 12;
	}

	int next_state_reservedop_dfa(int state, char symbol)
	{
		switch (state)
		{
		case 0:
		{
			if (symbol == '.')
				return 1;
			if (symbol == '=')
				return 3;
			if (symbol == '\\')
				return 4;
			if (symbol == '|')
				return 5;
			if (symbol == '<')
				return 6;
			if (symbol == '-')
				return 8;
			if (symbol == '@')
				return 10;
			if (symbol == '~')
				return 11;
		}
		case 1:
			if (symbol == '.')
				return 2;
			else
				return -1;
		case 3:
			if (symbol == '>')
				return 12;
			else
				return -1;
		case 6:
			if (symbol == '-')
				return 7;
			else
				return -1;
		case 8:
			if (symbol == '>')
				return 9;
			else
				return -1;
		default:
			return -1;
		}
	}

	Token_type get_token_type_by_final_reservedop_dfa_state(int state)
	{
		switch (state)
		{
		case 2:
			return Token_type::Two_dots;
		case 3:
			return Token_type::Equals;
		case 4:
			return Token_type::Backtick;
		case 5:
			return Token_type::Vertical_bar;
		case 7:
			return Token_type::Arrow_left;
		case 9:
			return Token_type::Arrow_right;
		case 10:
			return Token_type::At;
		case 11:
			return Token_type::Tilde;
		case 12:
			return Token_type::Instance_arrow;
		}

	}


	void handle_reservedop_dfa(int state, int i, const std::string &content, Token &token)
	{
		if (i > content.size())
		{
			return;
		}

		if (i == content.size())
		{
			if (is_final_reservedop_dfa_state(state))
			{
				token.type = get_token_type_by_final_reservedop_dfa_state(state);
			}
			return;
		}

		char c = content[i];
		int new_state = next_state_reservedop_dfa(state, c);
		if (new_state == -1)
			return;
		handle_reservedop_dfa(new_state, i + 1, content, token);

	}

	void check_var_reserverdop(std::string &content, Token &token)
	{

		handle_reservedop_dfa(0, 0, content, token);

		/*if (content == "..")
		{
			token.type = Token_type::Two_dots;
		}
		else if (content == "=")
		{
			token.type = Token_type::Equals;
		}
		else if (content == "\\")
		{
			token.type = Token_type::Backtick;
		}
		else if (content == "|")
		{
			token.type = Token_type::Vertical_bar;
		}
		else if (content == "<-")
		{
			token.type = Token_type::Arrow_left;
		}
		else if (content == "->")
		{
			token.type = Token_type::Arrow_right;
		}
		else if (content == "@")
		{
			token.type = Token_type::At;
		}
		else if (content == "~")
		{
			token.type = Token_type::Tilde;
		}
		else if (content == "=>")
		{
			token.type = Token_type::Instance_arrow;
		}*/
	}

	

	void check_con_reserverdid(std::string &content, Token &token)
	{
		if (content == ":")
		{
			token.type = Token_type::Colon;
		}
		else if (content == "::")
		{
			token.type = Token_type::Two_colons;
		}
	}

	void handle_qvarsym(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Qvarsym, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		if (c == ':')
		{
			handle_qconsym(code_data, tokens, symbolTable);
			return;
		}

		while (is_symbol(c) || c == ':')
		{
			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
				break;
		}

		check_var_reserverdop(content, token);

		if (!(token.type > Token_type::Reservedop_begin && token.type < Token_type::Reservedop_end))
		{
			int id = add_to_symbol_table(symbolTable, content);
			token.symbol_table_id = id;
		}

		tokens.push_back(token);
	}

	void handle_qconsym(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Qconsym, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		bool first = true;
		while (is_symbol(c) || c == ':')
		{
			if (first)
			{
				first = false;
				if (c != ':')
				{
					token_error(code_data, token, tokens);
					return;
				}
			}

			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
				break;
		}

		check_con_reserverdid(content, token);

		if (!(token.type > Token_type::Reservedop_begin && token.type < Token_type::Reservedop_end))
		{
			int id = add_to_symbol_table(symbolTable, content);
			token.symbol_table_id = id;
		}

		tokens.push_back(token);

	}


	void handle_numeric(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Integer, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		bool dot = false;
		while (is_digit(c))
		{
			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
				break;
		}
		if (c == '.' && code_data.current_line.size() > code_data.position + 1 && is_digit(code_data.current_line[code_data.position]))
		{
			token.type = Token_type::Float;
			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}
			while (is_digit(c))
			{
				content += c;
				if (code_data.current_line.size() > code_data.position)
					c = code_data.current_line[++code_data.position];
				else
					break;
			}
		}


		int id = add_to_symbol_table(symbolTable, content);
		token.symbol_table_id = id;
		tokens.push_back(token);
	}

	void handle_char(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::Char, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];	

		if (c == '\'')
		{
			content += '\'';
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}
		}
		else
		{
			token_error(code_data, token, tokens);
			return;
		}

		if (is_graphic(c))
		{
			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}

		}
		else
		{
			token_error(code_data, token, tokens);
			return;
		}

		if (c == '\'')
		{
			content += '\'';
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
		}
		else
		{
			token_error(code_data, token, tokens);
			return;
		}

		int id = add_to_symbol_table(symbolTable, content);
		token.symbol_table_id = id;
		tokens.push_back(token);
	}


	void handle_string(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		Token token = Token(Token_type::String, code_data.line, code_data.position, RAND_MAX);
		std::string content = "";

		char c = code_data.current_line[code_data.position];
		if (c == '"')
		{
			content += '"';
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}
		}
		else
		{
			token_error(code_data, token, tokens);
			return;
		}

		while (is_graphic(c))
		{
			content += c;
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}
		}

		if (c == '"')
		{
			content += '"';
			if (code_data.current_line.size() > code_data.position)
				c = code_data.current_line[++code_data.position];
			else
			{
				token_error(code_data, token, tokens);
				return;
			}
		}
		else
		{
			token_error(code_data, token, tokens);
			return;
		}

		int id = add_to_symbol_table(symbolTable, content);
		token.symbol_table_id = id;
		tokens.push_back(token);
	}



	void lex(std::string file_name, std::vector<Token> &tokens, std::vector<std::string> &symbolTable)
	{
		std::ifstream file_stream = std::ifstream(file_name);
		Code_data code_data;
		code_data.line = 1;
		code_data.position = 0;
		while (std::getline(file_stream, code_data.current_line))
		{
			while (code_data.current_line.size() > code_data.position)
			{
				if (ncomment_count)
				{
					handle_ncomment(code_data, tokens, symbolTable, comment_data);
				}

				char c = code_data.current_line[code_data.position];
				if (is_small(c))
				{
					handle_qvarid(code_data, tokens, symbolTable);
				}
				else if (is_large(c))
				{
					handle_qconid(code_data, tokens, symbolTable);
				}
				else if (is_symbol(c) || c == ':')
				{
					handle_qvarsym(code_data, tokens, symbolTable);
				}
				else if (c == '\'')
				{
					handle_char(code_data, tokens, symbolTable);
				}
				else if (c == '"')
				{
					handle_string(code_data, tokens, symbolTable);
				}
				else if (is_digit(c))
				{
					handle_numeric(code_data, tokens, symbolTable);
				}
				else if (c == '-')
				{
					if (code_data.current_line.size() > code_data.position + 1 && code_data.current_line[code_data.position + 1] == '-')
					{
						handle_comment(code_data, tokens, symbolTable);
					}
				}
				else if (c == '{')
				{
					if (code_data.current_line.size() > code_data.position + 1 && code_data.current_line[code_data.position + 1] == '-')
					{
						handle_ncomment(code_data, tokens, symbolTable);
					}
				}
				else if (is_special(c))
				{
					handle_special(code_data, tokens, symbolTable);
				}
				else if (is_whitechar(c))
				{
					code_data.position++;
					std::string s;
					s.push_back(c);
					tokens.push_back(Token(Token_type::Whitechar, code_data.line, code_data.position, add_to_symbol_table(symbolTable, s)));
				}
				else
				{
					//token error
				}

				if (code_data.broken_token)
				{
					code_data.broken_token = false;
					break;
				}
			}
			
			tokens.push_back(Token(Token_type::Whitechar, code_data.line, code_data.position, add_to_symbol_table(symbolTable, "\n")));
			code_data.line++;
			code_data.position = 0;
		}
	}
}
