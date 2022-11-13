#include "debug.hpp"
#include "Parser.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Parser::Parser():
	_lexer(),
	_currentBlock(NULL),
	_context(NONE),
	_serverBlockTmp(NULL)
{
	_initArrayParsingFunctions();
}

Parser::Parser(std::string configFile):
	_configFile(configFile),
	_lexer(configFile),
	_currentToken(_lexer.getTokens().begin()),
	_currentBlock(NULL),
	_context(NONE),
	_serverBlockTmp(NULL)
{
	//DEBUG("Creating array...");
	_initArrayParsingFunctions();
	//DEBUG("Parsing...");
	// printTokens();
	parseTokens();
	_lexer.closeFile();
	//DEBUG("Parsing done");
}

Parser::Parser(const Parser& other)
{
	*this = other;
}

Parser::~Parser()
{
	_deleteServers();
}

Parser&		Parser::operator=(const Parser& other)
{
	if (this != &other)
	{
		_configFile = other.getConfigFile();
		_lexer = other.getLexer();
		_currentToken = other.getCurrentToken();
		_servers = other.getServers();
		_currentServer = other.getCurrentServer();
		_currentBlock = other.getCurrentBlock();
		_parsingFunct = other.getParsingFunct();
		_context = other.getContext();
		_directive = other.getDirective();
	}
	return (*this);
}

/******************************************************************************/
/*                                   PARSER                                   */
/******************************************************************************/

void	Parser::parseFile(std::string configFile)
{
	_configFile = configFile;
	_lexer.openFile(configFile);
	_currentToken = _lexer.getTokens().begin();
	parseTokens();
}

void	Parser::_parseRule()
{
	Parser::listOfParsingFunctions::const_iterator	parseFunctIte;
	Token::tokenType								tokenType;

	tokenType = _currentToken->getType();
	if (tokenType == KEYWORD_SERVER)
		_throwErrorParsing("nested server");
	_checkDelimiter(_currentToken);
	parseFunctIte = _parsingFunct.find(tokenType);
	if (parseFunctIte == _parsingFunct.end())
		_throwErrorParsing(_unknownDirectiveMsg(_currentToken));
	_setDirective(tokenType);
	(this->*parseFunctIte->second)();
	if (tokenType != KEYWORD_LOCATION && tokenType != KEYWORD_LISTEN)
		_expectNextToken(SEMICOLON, _invalidNbOfArgumentsMsg());
}

void	Parser::_createNewLocation()
{
	blockPtr		newLocation;
	std::string		locationPath;
	std::string		lineNbrLocation;

	_directive = "location";
	_expectContext(SERVER, "nested location");
	lineNbrLocation	= _currentToken->getLineStr();
	_serverBlockTmp = _currentBlock;

	_expectNbOfArguments(1, EQUAL, BLOCK_START);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	locationPath = _currentToken->getValue();
	newLocation = new Block;
	_updateContext(LOCATION, newLocation);
	_parseBlock();

	_directive = "";
	if (!(_serverBlockTmp->insertLocation(locationPath, newLocation)))	
		_throwErrorParsing("duplicate location '" + locationPath + "'", lineNbrLocation);
	_updateContext(SERVER, _serverBlockTmp);
	_serverBlockTmp = NULL;
}

Parser::blockPtr	Parser::_createNewServer()
{
	blockPtr	newServer;

	if (_context != NONE)
		_throwErrorParsing("nested server");
	newServer = new Block;
	_updateContext(SERVER, newServer);
	return (newServer);
}

void	Parser::_parseBlock()
{
	_expectNextToken(BLOCK_START, _unexpectedValueMsg(_currentToken + 1));
	while (!_reachedEndOfBlock())
	{
		_getNextToken();
		_parseRule();
	}
	_expectNextToken(BLOCK_END, _unexpectedValueMsg(_currentToken + 1));
}

void	Parser::parseTokens()
{
	blockPtr	newServer;

	while (!_reachedEndOfTokens())
	{
		_expectNextToken(KEYWORD_SERVER, _keywordServerError());
		newServer = _createNewServer();
		_parseBlock();
		_completeLocationsBlock(newServer);
		_currentServer = _servers.insert(_servers.end(), newServer);
		_updateContext(NONE, NULL);
	}
	_configureVirtualHosts();
	// _checkDuplicatePorts();
}

/******************************************************************************/
/*                                PARSING RULES                               */
/******************************************************************************/

/* Context: Server */
void	Parser::_parseServerNameRule()
{
	_expectContext(SERVER);
	_expectNbOfArguments(1, MINIMUM, SEMICOLON);
	while (!_reachedEndOfDirective())
	{
		_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
		_currentBlock->setName(_currentToken->getValue());
	}
}

/* Context: Server */
void	Parser::_parseListenRule()
{
	int	port;

	_expectContext(SERVER);
	_expectNbOfArguments(1, MINIMUM, SEMICOLON);
	_getNextToken();
	switch (_currentToken->getType())
	{
		case VALUE:
			_setHost(_currentToken->getValue());
			if (_reachedEndOfDirective())
				break;	
			_expectNextToken(COLON, _invalidParameterMsg());
			/* fall through */
		case COLON:
			_expectNextToken(NUMBER, _invalidPortMsg());
			/* fall through */
		case NUMBER:
			// port = atoi(_currentToken->getValue().c_str());
			if (!convertPort(_currentToken->getValue(), &port))
				_throwErrorParsing(_invalidPortMsg());
			_currentBlock->setPort(port);
			break;
		case SEMICOLON:
			_throwErrorParsing(_invalidNbOfArgumentsMsg());
			/* fall through */
		default:
			_throwErrorParsing(_invalidValueMsg(_currentToken + 1));
			break;	
	}
	_checkDuplicatePorts(_currentBlock->getHost(), _currentBlock->getPort());
	_expectNextToken(SEMICOLON, _invalidParameterMsg());
}

/* Context: Server, Location */
void	Parser::_parseIndexRule()
{
	_expectNbOfArguments(1, MINIMUM, SEMICOLON);
	while (!_reachedEndOfDirective())
	{
		_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
		_currentBlock->setIndex(_currentToken->getValue());
	}
}

/* Context: Server, Location */
void	Parser::_parseRootRule()
{
	std::string	path;

	_expectNbOfArguments(1, EQUAL, SEMICOLON);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	path = _currentToken->getValue();
	_currentBlock->setRoot(path);
}

/* Context: Server, Location */
void	Parser::_parseAutoindexRule()
{
	_expectNbOfArguments(1, EQUAL, SEMICOLON);
	_getNextToken();
	if (_currentToken->getValue() == "on")
		return (_currentBlock->setAutoindex(true));
	if (_currentToken->getValue() == "off")
		return (_currentBlock->setAutoindex(false));
	_throwErrorParsing(_invalidValueMsg(_currentToken));
}

/* Context: Server, Location */
void	Parser::_parseErrorPageRule()
{
	int				code;
	std::string		page;

	_expectNbOfArguments(2, EQUAL, SEMICOLON);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	// code = atoi(_currentToken->getValue().c_str());
	if (!convertHttpCode(_currentToken->getValue(), &code) || code < 400 || code > 505)
		_throwErrorParsing(_invalidErrorCodeMsg(_currentToken));
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	page = _currentToken->getValue();
	_currentBlock->setErrorPage(code, page);
}

/* Context: Server, Location */
void	Parser::_parseMaxBodySizeRule()
{
	size_t	maxSize;

	_expectNbOfArguments(1, EQUAL, SEMICOLON);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	maxSize = atol(_currentToken->getValue().c_str());
	if (maxSize > DEFAULT_CLIENT_BODY_LIMIT)
		_throwErrorParsing("client_max_body_size too large");
	_currentBlock->setClientBodyLimit(maxSize);
}

/* Context: Server, Location */
void	Parser::_parseCgiRule()
{
	std::string	extension;
	std::string	path;

	_expectNbOfArguments(2, EQUAL, SEMICOLON);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	// if (_currentToken->getValue() != "php"	&& _currentToken->getValue() != ".php"
	// 	&& _currentToken->getValue() != ".py"	&& _currentToken->getValue() != "py")
	// 	_throwErrorParsing(_invalidValueMsg(_currentToken));
	extension = _currentToken->getValue();
	if (extension[0] && extension[0] == '.')
		extension.erase(0, 1);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	path = _currentToken->getValue();
	_currentBlock->setCgi(extension, path);
}

/* Context: Location */
void	Parser::_parseRedirectRule()
{
	int				code;
	std::string		uri;

	_expectContext(LOCATION);
	_expectNbOfArguments(2, EQUAL, SEMICOLON);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	if (!convertHttpCode(_currentToken->getValue(), &code) || code < 300 || code >= 400)
		_throwErrorParsing(_invalidValueMsg(_currentToken));
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	uri = _currentToken->getValue();
	_currentBlock->setRedirection(code, uri);
}

/* Context: Location */
void	Parser::_parseAllowedMethodRule()
{
	_expectContext(LOCATION);
	_expectNbOfArguments(1, MINIMUM, SEMICOLON);
	_currentBlock->initMethods(false);
	while (!_reachedEndOfDirective())
	{
		_getNextToken();
		if (g_httpMethod.isHttpMethod(_currentToken->getValue()) == false)
			_throwErrorParsing(_unknownMethodMsg(_currentToken));
		_currentBlock->setMethod(_currentToken->getValue(), true);
	}
}

/* Context: Location */
void	Parser::_parseUploadPathRule()
{
	// _expectContext(LOCATION);
	_expectNbOfArguments(1, EQUAL, SEMICOLON);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	_currentBlock->setUploadPath(_currentToken->getValue());
}

/* Context: Server */
void	Parser::_configureVirtualHosts()
{
	listOfServers::iterator		currentServer;
	listOfServers::iterator		nextServer;

	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++)
	{
		for (nextServer = currentServer + 1; nextServer != _servers.end(); nextServer++)
		{
			if (**currentServer == **nextServer)
			{
				(*currentServer)->setVirtualHost(*nextServer);
				_servers.erase(nextServer);
				nextServer--;
			}
		}
	}
}

void	Parser::_completeLocationsBlock(const blockPtr server)
{
	Block::listOfLocations::const_iterator	currentLocation;
	Block::listOfLocations					locations;

	locations = server->getLocations();
	if (locations.empty())
		return ;
	for (currentLocation = locations.begin(); currentLocation != locations.end(); currentLocation++)
	{
		/* We complete empty directives with server directives */
		currentLocation->second->completeLocationDirectives(*server);
	}
}

/******************************************************************************/
/*                                   CHECK                                    */
/******************************************************************************/

void	Parser::_checkDelimiter(Lexer::listOfTokens::const_iterator token)
{
	Token::tokenType	tokenType;

	tokenType = token->getType();
	if (tokenType == BLOCK_END || tokenType == BLOCK_START || tokenType == SEMICOLON)
		_throwErrorParsing(_unexpectedValueMsg(token), token->getLineStr());
}

void	Parser::_checkEndOfFile(Lexer::listOfTokens::const_iterator token)
{
	if (token == _lexer.getTokens().end())
		_throwErrorParsing(_unexpectedValueMsg(token));
}

void	Parser::_setHost(const std::string &token)
{
	size_t		found;
	size_t		pos;

	pos = 0;
	if (token == "localhost")
		return (_currentBlock->setHost("127.0.0.1"));
	if (token == "*")
		return (_currentBlock->setHost("0.0.0.0"));
	for (int i = 0; i < 4; i++)
	{
		found = token.find_first_not_of("0123456789", pos);
		if ((i < 3 && (found == std::string::npos || token[found] != '.')) 
			|| (i == 3 && found != std::string::npos)
			|| (atoi(token.substr(pos, found - pos).c_str()) > 255))
			_throwErrorParsing("host not found in '" + token + "' of the 'listen' directive");
		pos = found + 1;
	}
	_currentBlock->setHost(_currentToken->getValue());
}

void	Parser::_checkDuplicatePorts(const std::string& host, int port)
{
	listOfServers::iterator		currentServer;

	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++)
	{
		if ((*currentServer)->getHost() != host && (*currentServer)->getPort() == port)
		{
			_throwErrorParsing(_duplicatePortMsg((*currentServer)->getPort()));
		}
	}
}

/******************************************************************************/
/*                                  EXPECT                                    */
/******************************************************************************/

void	Parser::_expectNextToken(Token::tokenType expectedType, std::string errorMsg)
{
	if (!_getNextToken() || _currentToken->getType() != expectedType)
		_throwErrorParsing(errorMsg);
}

void	Parser::_expectNbOfArguments(int expectedNb, bool expectComp, Token::tokenType tokenType)
{
	int									count;
	Lexer::listOfTokens::const_iterator	ite;

	count = 0;
	_checkEndOfFile(_currentToken + 1);
	for (ite = _currentToken + 1; ite != _lexer.getTokens().end() && ite->getType() != tokenType; ite++)
	{
		_checkDelimiter(ite);
		count++;
	}
	if ((expectComp == EQUAL && count != expectedNb) || (expectComp == MINIMUM && count < expectedNb))
		_throwErrorParsing(_invalidNbOfArgumentsMsg(), ite->getLineStr());	
}

void	Parser::_expectContext(t_context expectedContext)
{
	if (_context != expectedContext)
		_throwErrorParsing(_directiveNotAllowedHereMsg());
}

void	Parser::_expectContext(t_context expectedContext, std::string errorMsg)
{
	if (_context != expectedContext)
		_throwErrorParsing(errorMsg);
}

/******************************************************************************/
/*                                  REACHED                                   */
/******************************************************************************/

bool	Parser::_reachedEndOfTokens()
{
	return (_currentToken != _lexer.getTokens().end() && _currentToken + 1 == _lexer.getTokens().end());
}

bool	Parser::_reachedEndOfBlock()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	_checkEndOfFile(_currentToken + 1);
	return (ite != _lexer.getTokens().end() && ite->getType() == BLOCK_END);
}

bool	Parser::_reachedEndOfDirective()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	_checkEndOfFile(_currentToken + 1);
	return (ite != _lexer.getTokens().end() && ite->getType() == SEMICOLON);
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

bool	Parser::_getNextToken()
{
	if (!_reachedEndOfTokens())
		_currentToken++;
	return (_currentToken != _lexer.getTokens().end());
}

void	Parser::_initArrayParsingFunctions()
{
	_parsingFunct[KEYWORD_SERVER_NAME] = &Parser::_parseServerNameRule;
	_parsingFunct[KEYWORD_LISTEN] = &Parser::_parseListenRule;
	_parsingFunct[KEYWORD_ROOT] = &Parser::_parseRootRule;
	_parsingFunct[KEYWORD_INDEX] = &Parser::_parseIndexRule;
	_parsingFunct[KEYWORD_AUTOINDEX] = &Parser::_parseAutoindexRule;
	_parsingFunct[KEYWORD_BODY_LIMIT] = &Parser::_parseMaxBodySizeRule;
	_parsingFunct[KEYWORD_CGI] = &Parser::_parseCgiRule;
	_parsingFunct[KEYWORD_ERROR_PAGE] = &Parser::_parseErrorPageRule;
	_parsingFunct[KEYWORD_REDIRECT] = &Parser::_parseRedirectRule;
	_parsingFunct[KEYWORD_ALLOWED_METHOD] = &Parser::_parseAllowedMethodRule;
	_parsingFunct[KEYWORD_UPLOAD_PATH] = &Parser::_parseUploadPathRule;
	_parsingFunct[KEYWORD_LOCATION] = &Parser::_createNewLocation;
}

void	Parser::_updateContext(t_context currentContext, blockPtr currentBlock)
{
	_context = currentContext;
	_currentBlock = currentBlock;
	if (currentBlock)
		_currentBlock->setContext(currentContext);
}

void	Parser::_setDirective(Token::tokenType type)
{
	Lexer::listOfTokenTypes::const_iterator		ite;
	Lexer::listOfTokenTypes						tokenTypes;

	tokenTypes = _lexer.getTokenTypes();
	for (ite = tokenTypes.begin(); ite != tokenTypes.end(); ite++)
	{
		if (ite->second == _currentToken->getType())
		{
			_directive = ite->first;
			break ;
		}
	}
	if (_currentBlock->directiveIsSet(type))
		_throwErrorParsing("directive " + _directive + " is already set");
	_currentBlock->setDirective(type);
}

bool	Parser::_isDirective(Token::tokenType type)
{
	Lexer::listOfTokenTypes::const_iterator		ite;
	Lexer::listOfTokenTypes						tokenTypes;

	tokenTypes = _lexer.getTokenTypes();
	for (ite = tokenTypes.begin(); ite != tokenTypes.end(); ite++)
	{
		_lexer.printType(type);
		if (ite->second == type)
		{
			_directive = ite->first;
			return (true);
		}
	}
	return (false);
}

void	Parser::_deleteServers()
{
	for (_currentServer = _servers.begin(); _currentServer != _servers.end(); _currentServer++)
	{
		if (*_currentServer)
			delete (*_currentServer);
	}
}

void	Parser::_deleteBlock(Block* block)
{
	if (block)
	{
		delete block;
		block = NULL;
	}
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Parser::getConfigFile() const
{
	return (_configFile);
}

Lexer	Parser::getLexer() const
{
	return (_lexer);
}

Lexer::listOfTokens::const_iterator		Parser::getCurrentToken() const
{
	return (_currentToken);
}

const Parser::listOfServers&	Parser::getServers() const
{
	return (_servers);
}

Parser::listOfServers::const_iterator	Parser::getCurrentServer() const
{
	return (_currentServer);
}

Parser::blockPtr	Parser::getCurrentBlock() const
{
	return (_currentBlock);
}

Parser::listOfParsingFunctions		Parser::getParsingFunct() const
{
	return (_parsingFunct);
}

t_context	Parser::getContext() const
{
	return (_context);
}

std::string		Parser::getDirective() const
{
	return (_directive);
}

/******************************************************************************/
/*                                   ERROR                                    */
/******************************************************************************/

std::string Parser::_invalidErrorCodeMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("value '" + token->getValue() + "' must be between 300 and 527");
}

std::string Parser::_unexpectedValueMsg(Lexer::listOfTokens::const_iterator token)
{
	if (token != _lexer.getTokens().end())
		return ("unexpected '" + token->getValue() + "'");
	return ("unexpected end of file");
}

std::string Parser::_unknownMethodMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("unknown method '" + token->getValue() + "' in '" + getDirective() + "' directive");
}

std::string Parser::_unknownDirectiveMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("unknown directive '" + token->getValue() + "'");
}

std::string Parser::_keywordServerError()
{
	Lexer::listOfTokens::const_iterator		nextToken;

	nextToken = _currentToken + 1;
	_checkDelimiter(nextToken);
	if (_isDirective(nextToken->getType()))
	{
		return (_directiveNotAllowedHereMsg());
	}
	return (_unknownDirectiveMsg(nextToken));
}

std::string Parser::_directiveNotAllowedHereMsg()
{
	return ("'" + getDirective() + "' directive is not allowed here");
}

std::string	Parser::_invalidNbOfArgumentsMsg()
{
	return ("invalid number of arguments in '" + getDirective() + "' directive");
}

std::string	Parser::_invalidParameterMsg()
{
	std::string	incorrectParam;

	incorrectParam = "";
	if ((_currentToken + 1) != _lexer.getTokens().end())
		incorrectParam = (_currentToken + 1)->getValue();
	return ("invalid parameter '" + incorrectParam + "'");
}

std::string	Parser::_invalidPortMsg()
{
	return ("invalid port of the 'listen' directive");
}

std::string	Parser::_invalidValueMsg(Lexer::listOfTokens::const_iterator token)
{
	std::string	incorrectValue;

	incorrectValue = "";
	if (token != _lexer.getTokens().end())
		incorrectValue = token->getValue();
	return ("invalid value '" + incorrectValue + "' in '" + getDirective() + "' directive");
}

std::string	Parser::_invalidPathMsg()
{
	std::string	incorrectValue;

	incorrectValue = "";
	if ((_currentToken) != _lexer.getTokens().end())
		incorrectValue = (_currentToken)->getValue();
	return ("invalid path '" + incorrectValue + "'");
}

std::string	Parser::_duplicatePortMsg(int port)
{
	return ("duplicate port '" + convertNbToString(port) + "'");
}

void	Parser::_throwErrorParsing(std::string errorMsg)
{
	std::string	message;
	std::string	lineNbr;

	if (_currentToken + 1 == _lexer.getTokens().end())
		lineNbr = _lexer.getLineCountStr();
	else
		lineNbr = _currentToken->getLineStr();
	message = "Webserv error: " + errorMsg;
	message += " in " + getConfigFile() + ":" + lineNbr;
	_deleteBlock(_currentBlock);
	_deleteBlock(_serverBlockTmp);
	_deleteServers();
	throw (std::runtime_error(message));
}

void	Parser::_throwErrorParsing(std::string errorMsg, std::string lineNbr)
{
	std::string message;

	message = "Webserv error: " + errorMsg;
	message += " in " + getConfigFile() + ":" + lineNbr;
	_deleteBlock(_currentBlock);
	_deleteBlock(_serverBlockTmp);
	_deleteServers();
	throw (std::runtime_error(message));
}

/******************************************************************************/
/*                                  DISPLAY                                   */
/******************************************************************************/

void	Parser::_printArrayParsingFunctions()
{
	Parser::listOfParsingFunctions::iterator	ite;

	for (ite = _parsingFunct.begin(); ite != _parsingFunct.end(); ite++)
		(this->*ite->second)();
}

void	Parser::_printCurrentToken()
{
	std::cout << YELLOW << "Current Token : [";
	std::cout << _lexer.printType(_currentToken->getType()) << " : ";
	std::cout << _currentToken->getValue() << "]" << RESET << std::endl;
}

void	Parser::printTokens()
{
	Lexer::listOfTokens::const_iterator		ite;

	std::cout << std::endl  << "  >>> PARSER | List of tokens <<< " << std::endl;
	for (ite = _lexer.getTokens().begin(); ite != _lexer.getTokens().end(); ite++)
	{
		std::cout << "[" << _lexer.printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]";
		std::cout << " : " << ite->getLineStr() << "]" << std::endl;
	}
}

void	Parser::displayServersParams() const
{
	listOfServers::const_iterator	currentServer;
	int								count;

	count = 1;
	std::cout << GREY << std::endl << "................................";
	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++, count++)
		(*currentServer)->displayParams(count);
	std::cout << "................................" << RESET << std::endl;
}
