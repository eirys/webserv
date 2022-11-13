#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Block.hpp"

extern HttpMethod	g_httpMethod;

/******************************************************************************/
/*                               CLASS PARSER                                 */
/******************************************************************************/

typedef enum e_comp
{
	EQUAL	= 0,
	MINIMUM	= 1
} t_comp;

class Parser
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<Block *>						listOfServers;
		typedef	Block*										blockPtr;
		typedef void (Parser::*parsingFunction)();
		typedef std::map<Token::tokenType, parsingFunction>	listOfParsingFunctions;


	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string							_configFile;
		Lexer								_lexer;
		Lexer::listOfTokens::const_iterator	_currentToken;
		listOfServers						_servers;
		listOfServers::const_iterator		_currentServer;
		blockPtr							_currentBlock;
		listOfParsingFunctions				_parsingFunct;
		t_context							_context;
		std::string							_directive;
		blockPtr							_serverBlockTmp;
		
	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Parser();
		Parser(std::string configFile);
		Parser(const Parser& other);
		~Parser();
		Parser&								operator=(const Parser& other);

						/*-------    Parser   ------*/
		void								parseFile(std::string configFile);
		void								parseTokens();

						/*-------    Getter   ------*/
		std::string							getConfigFile() const;
		Lexer								getLexer() const;
		Lexer::listOfTokens::const_iterator	getCurrentToken() const;
		const listOfServers&				getServers() const;
		listOfServers::const_iterator		getCurrentServer() const;
		blockPtr							getCurrentBlock() const;
		listOfParsingFunctions				getParsingFunct() const;
		t_context							getContext() const;
		std::string							getDirective() const;

						/*-------    Display   ------*/
		void								printTokens();
		void								displayServersParams() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------    Parser   ------*/
		void								_parseBlock();
		blockPtr							_createNewServer();
		void								_createNewLocation();
		void								_parseRule();
		void								_completeLocationsBlock(const blockPtr server);

						/*-----  Parsing rules -----*/
		void								_parseServerNameRule();
		void								_parseListenRule();
		void								_parseRootRule();
		void								_parseIndexRule();
		void								_parseAutoindexRule();
		void								_parseMaxBodySizeRule();
		void								_parseCgiRule();
		void								_parseErrorPageRule();
		void								_parseRedirectRule();
		void								_parseAllowedMethodRule();
		void								_parseUploadPathRule();

						/*-------  Configure  ------*/
		void								_configureVirtualHosts();

						/*-------    Check    ------*/
		void								_checkDelimiter(Lexer::listOfTokens::const_iterator token);
		void								_checkEndOfFile(Lexer::listOfTokens::const_iterator token);
		void								_checkDuplicatePorts(const std::string& host, int port);

		void								_setHost(const std::string &token);

						/*-------    Expect    ------*/
		void								_expectNextToken(Token::tokenType expectedType, std::string errorMsg);
		void								_expectNbOfArguments(int expectedNb, bool expectComp, Token::tokenType tokenType);
		void								_expectContext(t_context expectedContext);
		void								_expectContext(t_context expectedContext, std::string errorMsg);

						/*-------   Reached    ------*/
		bool								_reachedEndOfTokens();
		bool								_reachedEndOfDirective();
		bool								_reachedEndOfBlock();

						/*-------    Utils    ------*/
		bool								_getNextToken();
		void								_initArrayParsingFunctions();
		void								_updateContext(t_context currentContext, blockPtr currentBlock);
		void								_setDirective(Token::tokenType type);
		bool								_isDirective(Token::tokenType type);
		void								_deleteServers();
		void								_deleteBlock(Block* block);

						/*-------     Error    ------*/
		std::string							_unexpectedValueMsg(Lexer::listOfTokens::const_iterator token);
		std::string							_unknownDirectiveMsg(Lexer::listOfTokens::const_iterator token);
		std::string							_unknownMethodMsg(Lexer::listOfTokens::const_iterator token);
		std::string							_keywordServerError();
		std::string							_directiveNotAllowedHereMsg();
		std::string							_invalidValueMsg(Lexer::listOfTokens::const_iterator token);
		std::string							_invalidParameterMsg();
		std::string							_invalidErrorCodeMsg(Lexer::listOfTokens::const_iterator token);
		std::string							_invalidPortMsg();
		std::string							_invalidPathMsg();
		std::string							_invalidNbOfArgumentsMsg();
		std::string							_duplicatePortMsg(int port);
		void								_throwErrorParsing(std::string errorMsg);
		void								_throwErrorParsing(std::string errorMsg, std::string lineNbr);

						/*-------    Display   ------*/
		void								_printCurrentToken();
		void								_printArrayParsingFunctions();
};

#endif
