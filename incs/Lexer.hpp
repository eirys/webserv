#ifndef LEXER_HPP
# define LEXER_HPP

# include "Token.hpp"

/******************************************************************************/
/*                                CLASS LEXER                                 */
/******************************************************************************/

class	Lexer
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<Token>							listOfTokens;
		typedef std::map<std::string, Token::tokenType>		listOfTokenTypes;

	private :
	/**********************     MEMBER VARIABLES     ********************/
		std::ifstream					_file;
		listOfTokens					_tokens;
		listOfTokenTypes				_tokenTypes;
		unsigned int					_lineCount;

	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Lexer();
		Lexer(std::string configFile);
		Lexer(const Lexer& other);
		~Lexer();
		Lexer&							operator=(const Lexer& other);

						/*-------      File      -------*/
		void							openFile(std::string configFile);
		void							checkFile(std::string configFile);
		void							closeFile();

						/*-------     Getter      ------*/
		const std::ifstream&			getFile() const;
		const listOfTokens&				getTokens() const;
		listOfTokenTypes				getTokenTypes() const;
		size_t							getLineCount() const;
		std::string						getLineCountStr() const;

						/*-------     Display      ------*/
		void							printTokens();
		std::string						printType(Token::tokenType type);

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------     Lexer      ------*/
		void							_readFile();
		void							_getToken(char character, bool isDirective);
		void							_getValue(std::string &token);
		void							_getDelimiter();

						/*-------   Token Type  ------*/
		bool							_tokenIsNumber(const std::string &token);
		bool							_tokenIsSize(const std::string &token);
		bool							_tokenIsPath(const std::string &token);
		bool							_tokenIsAddress(const std::string &token);
		std::string						_handleSize(std::string &token);

						/*-------     Utils      ------*/
		char							_getNextCharacter();
		bool							_reachedEndOfFile();
		void							_ignoreComments(char character);
		bool							_isDelimiter(char character);
		void							_buildTokenTypeArray();

						/*-------      Error     ------*/
		void							_throwErrorLexer(std::string errorMsg);
};

#endif
