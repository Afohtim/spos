#pragma once
#include "CodeData.h"

#include <string>
#include <vector>

namespace lexer 
{
	enum class Token_type : uint8_t
	{
		//Whitespaces
		Whitespace_begin,

			//Whitechars
			Whitechar,

			//Comments
			Comments_begin,

			Comment,
			Ncomment,

			Comments_end,

		Whitespace_end,

		//Lexemes
		Lexemes_begin,
		

			//Identifiers
			Qvarid,
			Qconid,

			//Operator symbols
			Qvarsym,
			Qconsym,

			//Literals
			Literals_begin,

				Char,
				String,
				Integer,
				Float,

			Literals_end,

			//Special 
			Special_begin,

				Parenthese_left,
				Parenthese_right,
				Comma,
				Semicolon,
				Bracket_left,
				Bracket_right,
				Backtick,
				Brace_left,
				Brace_right,

			Special_end,

			//Reserved operators
			Reservedop_begin,

				Two_dots,
				Colon,
				Two_colons,
				Equals,
				Backslash,
				Vertical_bar,
				Arrow_left,
				Arrow_right,
				At,
				Tilde,
				Instance_arrow,

			Reservedop_end,
		
			//Reserved identifiers
			Reservedid_begin,

				Case,
				Class,
				Data,
				Default,
				Deriving,
				Do,
				Else,
				If,
				Import,
				In,
				Infix,
				Infixl,
				Infixr,
				Instance,
				Let,
				Module,
				Newtype,
				Of,
				Then,
				Type,
				Where,
				Underscore,

			Reservedid_end,
			Token_error,

		Lexemes_end
	};

	constexpr char const * Token_to_string[(int) Token_type::Lexemes_end + 1] =
	{
		//Whitespaces
		"Whitespace_begin",

			//Whitechars
			"Whitechar",

			//Comments
			"Comments_begin",

			"Comment",
			"Ncomment",

			"Comments_end",

		"Whitespace_end",

		//Lexemes
		"Lexemes_begin",


		//Identifiers
		"Qvarid",
		"Qconid",

		//Operator symbols
		"Qvarsym",
		"Qconsym",

		//Literals
		"Literals_begin",

			"Char",
			"String",
			"Integer",
			"Float",

		"Literals_end",

		//Special 
		"Special_begin",

			"(",
			")",
			",",
			";",
			"[",
			"]",
			"`",
			"{",
			"}",

		"Special_end",

		//Reserved operators
		"Reservedop_begin",

			"..",
			":",
			"::",
			"=",
			"\\",
			"|",
			"<-",
			"->",
			"@",
			"~",
			"=>",

		"Reservedop_end",

		//Reserved identifiers
		"Reservedid_begin",

			"case",
			"class",
			"data",
			"default",
			"deriving",
			"do",
			"else",
			"if",
			"import",
			"in",
			"infix",
			"infixl",
			"infixr",
			"instance",
			"let",
			"module",
			"newtype",
			"of",
			"then",
			"type",
			"where",
			"_",

		"Reservedid_end",
		"Token error",

	"Lexemes_end"
	};

	struct Token
	{
		Token(){}
		Token(Token_type type, int line, int position, int symbol_table_id) : type(type), line(line), position(position), symbol_table_id(symbol_table_id){}

		Token_type type;
		int line;
		int position;
		int symbol_table_id;
	};

	void lex(std::string file_name, std::vector<Token> &tokens, std::vector<std::string> &symbolTable);
	bool is_special(char);
	void handle_qconsym(Code_data &code_data, std::vector<Token> &tokens, std::vector<std::string> &symbolTable);
}