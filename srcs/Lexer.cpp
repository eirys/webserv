#include "Lexer.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Lexer::Lexer(): _lineCount(1)
{
	_buildTokenTypeArray();
	_tokens.insert(_tokens.end(), Token(FILE_START, "", 0));
}

Lexer::Lexer(std::string configFile): _lineCount(1)
{
	_buildTokenTypeArray();
	_tokens.insert(_tokens.end(), Token(FILE_START, "", 0));
	openFile(configFile);
}

Lexer::Lexer(const Lexer& other)
{
	*this = other;
}

Lexer::~Lexer()
{
	this->closeFile();
}

Lexer&	Lexer::operator=(const Lexer& other)
{
	if (this != &other)
	{
		_tokens = other.getTokens();
		_tokenTypes = other.getTokenTypes();
		_lineCount = other.getLineCount();
	}
	return (*this);
}

/******************************************************************************/
/*                                   LEXER                                    */
/******************************************************************************/

void	Lexer::_readFile()
{
	char	character;
	bool	isDirective;

	isDirective = true;
	while (!_file.eof())
	{
		character = _file.peek();
		if (character == '#')
			_ignoreComments(character);
		else if (isspace(character))
			_getNextCharacter();
		else if (_isDelimiter(character))
		{
			_getDelimiter();
			isDirective = true;
		}
		else
		{
			_getToken(character, isDirective);
			isDirective = false;
		}
	}
}

void	Lexer::_getToken(char character, bool isDirective)
{
	std::string					token;
	listOfTokenTypes::iterator	ite;

	token = "";
	while (!_reachedEndOfFile() && !isspace(character) && !_isDelimiter(character) && character != '#')
	{
		token += _getNextCharacter();
		character = _file.peek();
	}
	if (token != "")
	{
		ite = _tokenTypes.find(token);
		if (ite != _tokenTypes.end() && isDirective)
			_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token, _lineCount));
		else
			_getValue(token);
	}
}

void	Lexer::_getValue(std::string &token)
{
	t_tokenType	type;

	type = VALUE;
	if (_tokenIsNumber(token))
		type = NUMBER;
	else if (_tokenIsSize(token))
	{
		token = _handleSize(token);
		type = NUMBER;
	}
	// else if (_tokenIsPath(token))
	// 	type = PATH;
	_tokens.insert(_tokens.end(), Token(type, token, _lineCount));
}

void	Lexer::_getDelimiter()
{
	std::string		token;

	token = "";
	token += _getNextCharacter();
	_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token, _lineCount));
}

/******************************************************************************/
/*                                TOKEN TYPE                                  */
/******************************************************************************/

bool	Lexer::_isDelimiter(char character)
{
	return (character == ';' || character == '{' || character == '}' || character == ':');
}

bool	Lexer::_tokenIsNumber(const std::string &token)
{
	return (token.find_first_not_of("0123456789") == std::string::npos);
}

std::string		Lexer::_handleSize(std::string &token)
{
	char			lastCharacter;
	std::string		size;

	lastCharacter = token[token.size() - 1];
	size = token.substr(0, token.size() - 1);
	if (lastCharacter == 'k' || lastCharacter == 'K')
		size += "000";
	if (lastCharacter == 'm' || lastCharacter == 'M')
		size += "000000";
	if (lastCharacter == 'g' || lastCharacter == 'G')
		size += "000000000";
	return (size);
}

bool	Lexer::_tokenIsSize(const std::string &token)
{
	size_t	found;
	char	lastCharacter;

	found = token.find_first_not_of("0123456789");
	lastCharacter = token[token.size() - 1];
	if (found != std::string::npos && found == token.size() - 1)
		return (lastCharacter == 'k' || lastCharacter == 'K'
			|| lastCharacter == 'm' ||  lastCharacter == 'M'
			|| lastCharacter == 'g' || lastCharacter == 'G');
	return (false);
}

// bool	Lexer::_tokenIsPath(const std::string &token)
// {
// 	return (token.find("/") != std::string::npos);
// }


/******************************************************************************/
/*                                    FILE                                    */
/******************************************************************************/

void	Lexer::_throwErrorLexer(std::string errorMsg)
{
	std::string message;

	message = "Webserv error: " + errorMsg;
	throw (std::runtime_error(message));
}

void	Lexer::checkFile(std::string configFile)
{
	struct stat		statStruct;
	size_t			filenameLength;

	filenameLength = configFile.length();
	if (!(filenameLength > 5 && configFile.substr(filenameLength - 5) == ".conf"))
		_throwErrorLexer("invalid extension, the configuration file must end with '.conf'");
	if (stat(configFile.c_str(), &statStruct) != 0)
		_throwErrorLexer("file '" + configFile + "' not found");
	if (statStruct.st_mode & S_IFDIR)
		_throwErrorLexer("file '" + configFile + "' is a directory");
	if (access(configFile.c_str(), F_OK) < 0)
		_throwErrorLexer("insufficient permission to open file '" + configFile + "'");
}

void	Lexer::openFile(std::string configFile)
{
	checkFile(configFile);
	_file.open(configFile.c_str());
	if (!_file || !_file.is_open())
		_throwErrorLexer("cannot open file '" + configFile + "'");
	_readFile();
}

void	Lexer::closeFile()
{
	if (_file.is_open())
		_file.close();
}


/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

char	Lexer::_getNextCharacter()
{
	char nextCharacter;
	
	nextCharacter = _file.get();
	if (nextCharacter == '\n')
		_lineCount++;
	return (nextCharacter);
}

bool	Lexer::_reachedEndOfFile()
{
	return (_file.eof());
}

void	Lexer::_ignoreComments(char character)
{
	while (!_reachedEndOfFile() && character != '\n')
		character = _getNextCharacter();
}

void	Lexer::_buildTokenTypeArray(void)
{
	_tokenTypes["{"] = BLOCK_START;
	_tokenTypes["}"] = BLOCK_END;
	_tokenTypes[";"] = SEMICOLON;
	_tokenTypes[":"] = COLON;
	_tokenTypes["server"] = KEYWORD_SERVER;
	_tokenTypes["location"] = KEYWORD_LOCATION;
	_tokenTypes["server_name"] = KEYWORD_SERVER_NAME;
	_tokenTypes["listen"] = KEYWORD_LISTEN;
	_tokenTypes["root"] = KEYWORD_ROOT;
	_tokenTypes["index"] = KEYWORD_INDEX;
	_tokenTypes["autoindex"] = KEYWORD_AUTOINDEX;
	_tokenTypes["cgi"] = KEYWORD_CGI;
	_tokenTypes["error_page"] = KEYWORD_ERROR_PAGE;
	_tokenTypes["redirect"] = KEYWORD_REDIRECT;
	_tokenTypes["client_max_body_size"] = KEYWORD_BODY_LIMIT;
	_tokenTypes["allowed_method"] = KEYWORD_ALLOWED_METHOD;
	_tokenTypes["upload_path"] = KEYWORD_UPLOAD_PATH;
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

const std::ifstream&	Lexer::getFile() const
{
	return (_file);
}

const Lexer::listOfTokens&		Lexer::getTokens() const
{
	return (_tokens);
}

Lexer::listOfTokenTypes		Lexer::getTokenTypes() const
{
	return (_tokenTypes);
}

size_t	Lexer::getLineCount() const
{
	return (_lineCount);
}

std::string		Lexer::getLineCountStr() const
{
	return (convertSizeToString(_lineCount));
}


/******************************************************************************/
/*                                  DISPLAY                                   */
/******************************************************************************/

void	Lexer::printTokens()
{
	listOfTokens::iterator	ite;

	std::cout << std::endl  << "  >>> LEXER | List of tokens <<< " << std::endl;
	for (ite = _tokens.begin(); ite != _tokens.end(); ite++)
	{
		std::cout << "[";
		printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]" << std::endl;
	}
}

std::string		Lexer::printType(Token::tokenType type)
{
	if (type == FILE_START)
		return ("FILE_START");
	else if (type == BLOCK_START)
		return ("BLOCK_START");
	else if (type == BLOCK_END)
		return ("BLOCK_END");
	else if (type == SEMICOLON)
		return ("SEMICOLON");
	else if (type == COLON)
		return ("COLON");
	else if (type == KEYWORD_SERVER)
		return ("SERVER");
	else if (type == KEYWORD_LOCATION)
		return ("LOCATION");
	else if (type == KEYWORD_SERVER_NAME)
		return ("SERVER_NAME");
	else if (type == KEYWORD_LISTEN)
		return ("LISTEN");
	else if (type == KEYWORD_ROOT)
		return ("ROOT");
	else if (type == KEYWORD_INDEX)
		return ("INDEX");
	else if (type == KEYWORD_AUTOINDEX)
		return ("AUTOINDEX");
	else if (type == KEYWORD_CGI)
		return ("CGI");
	else if (type == KEYWORD_ERROR_PAGE)
		return ("ERROR_PAGE");
	else if (type == KEYWORD_REDIRECT)
		return ("REDIRECT");
	else if (type == KEYWORD_BODY_LIMIT)
		return ("BODY_LIMIT");
	else if (type == KEYWORD_ALLOWED_METHOD)
		return ("ALLOWED_METHOD");
	else if (type == KEYWORD_UPLOAD_PATH)
		return ("UPLOAD_PATH");
	else if (type == NUMBER)
		return ("NUMBER");
	else if (type == PATH)
		return("PATH");
	return ("VALUE");
}
