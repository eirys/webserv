#include "Request.hpp"

/* The HTTP GET request method is used to request a resource from the server.
The GET request should only receive data (the server must not change its state).
The HTTP POST method is used to create or add a resource on the server.
The POST method asks the web server to accept the data contained in the
body of the message. The data type in the HTTP POST body is indicated by the
Content-Type header.
- request line / status line
- HTTP headers [Header: value]
- HTTP body
*/

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Request::Request():
	_request(""), _fd(-1)
{
	_initVariables();
	_initParsingFunct();
}

Request::Request(const std::string& buffer, int clientfd):
	_request(buffer), _fd (clientfd), _raw(buffer)
{
	// DEBUG("Request: " + _request);
	_initVariables();
	_initParsingFunct();
}

void	Request::_initVariables()
{
	_requestStatus = INCOMPLETE_REQUEST;
	_statusCode = OK;
	_method = NO_METHOD;
	_path = "";
	_httpProtocol = "";
	_bodySize = 0;
	_body = "";
	_chunkedTransfer = false;
	_host = "";
	_port = UNDEFINED_PORT;
	_payloadSize = 0;
	_headerIsParsed = false;
}

Request::Request(const Request &other)
{
	*this = other;
}

Request::~Request() {}

Request&	Request::operator=(const Request &other)
{
	if (this != &other)
	{
		_request = other.getRequest();
		_requestStatus = other.getRequestStatus();
		_statusCode = other.getStatusCode(),
		_method = other.getMethod();
		_path = other.getPath();
		_httpProtocol = other.getHttpProtocol();
		_headers = other.getHeaders();
		_bodySize = other.getBodySize();
		_body = other.getBody();
		_parsingFunct = other.getParsingFunct();
		_chunkedTransfer = other.getChunkedTransfer();
		_host = other.getHost();
		_port = other.getPort();
		_payloadSize = other.getPayloadSize();
		_fd = other.getFd();
		_raw = other.getRawRequest();
		_cookies = other.getCookies();
	}
	return (*this);
}

/******************************************************************************/
/*                                  PARSING                                   */
/******************************************************************************/

t_requestStatus	Request::parseRequest()
{
	// displayMsg("Request: " + _request, LIGHT_BLUE);
	try
	{
		if (_request.find("\r\n\r\n") == std::string::npos)
		{
			/* Header is incomplete */
			return (_requestStatus);
		}
		if (_chunkedTransfer && _requestStatus != COMPLETE_REQUEST)
			_decodeChunks();
		else
		{
			if (!_headerIsParsed)
				_runParsingFunctions();
			_parseBody();
		}
	}
	catch(const t_statusCode& errorCode)
	{
		_statusCode = errorCode;
		_requestStatus = COMPLETE_REQUEST;
	}
	return (_requestStatus);
}

void	Request::_runParsingFunctions()
{
	Request::listOfParsingFunctions::const_iterator	currentFunct;

	for (currentFunct = _parsingFunct.begin();
		_requestStatus != COMPLETE_REQUEST && currentFunct != _parsingFunct.end();
			currentFunct++)
	{
		(this->**currentFunct)();
	}
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
	_raw += buffer;
}

void	Request::_parseMethod()
{
	std::string method;

	_getNextWord(method, " ");
	if (g_httpMethod.isHttpMethod(method) == false)
	{
		throw (NOT_IMPLEMENTED);
	}
	_method = g_httpMethod.getMethod(method);
}

void	Request::_parsePath()
{
	std::string		path;
	size_t			pos;

	_getNextWord(path, " ");
	if (path == "" || path[0] != '/')
		throw (BAD_REQUEST);
	if (path.length() > 2048) /* Maximum URL Length */
		throw (URI_TOO_LONG);
	/* Search for query */
	pos = path.find("?");
	if (pos != std::string::npos)
	{
		_query = path.substr(pos + 1);
		path.erase(pos);
		if (_query.length() > 255) /* limited by the DNS */
			throw (URI_TOO_LONG);
	}
	_path = path;
}

void	Request::_parseHttpProtocol()
{
	std::string httpProtocol;
	
	_getNextWord(httpProtocol, "\r\n");
	if (httpProtocol.find("HTTP") == std::string::npos)
		throw (BAD_REQUEST);
	if (httpProtocol != "HTTP/1.1")
		throw (HTTP_VERSION_NOT_SUPPORTED);
	_httpProtocol = httpProtocol;
}

void	Request::_parseHeaders()
{
	std::string		headerName;
	std::string		headerValue;
	size_t			pos;

	pos = 0;
	while (pos != std::string::npos && _request.find("\r\n"))
	{
		pos = _getNextWord(headerName, ":");
		if (pos == std::string::npos)
			break ;
		toLowerStr(&headerName); /* Case-insensitive */
		_getNextWord(headerValue, "\r\n");
		trimSpacesStr(&headerValue); /* We retrieve spaces around the value */
		if (_headerIsSet(headerName)) /* Check duplicate headers */
			throw (BAD_REQUEST);
		_headers[headerName] = headerValue;
		if (_payloadSize > 8192) /* Check if request header fields is too large (> 8K) */
			throw (PAYLOAD_TOO_LARGE);
	}
	_getNextWord(headerName, "\r\n");
}

bool	Request::_parseHostHeader()
{
	Request::listOfHeaders::const_iterator	ite;
	size_t									pos;

	ite = _headers.find("host");
	if (ite == _headers.end())
		return (false);
	_host = ite->second;
	pos = _host.find(":");
	if (pos == std::string::npos)
		return (true);
	_host = _host.substr(0, pos);
	if (pos + 1 != std::string::npos)
		return (convertPort(ite->second.substr(pos + 1), &_port));
	return (true);
}

void	Request::_checkContentLength()
{
	std::string	contentLength;
	size_t		size;

	if (_headers.find("content-length") == _headers.end())
		return ;
	contentLength = _headers["content-length"];
	if (contentLength.find_first_not_of("0123456789") != std::string::npos)
		throw (BAD_REQUEST);
	if (contentLength == "0")
	{
		_bodySize = 0;
		return ;
	}
	size = std::strtoul(contentLength.c_str(), NULL, 10);
	if (!size || size == ULONG_MAX)
		throw (BAD_REQUEST);
	_bodySize = size;
}

void	Request::_checkHeaders()
{
	Request::listOfHeaders::const_iterator	ite;
	std::string								contentLength;

	if (!_parseHostHeader())
		throw (BAD_REQUEST);
	_parseCookies();
	_checkContentLength();
	if (_method != POST)
	{
		_headerIsParsed = true;
		return ;
	}
	ite = _headers.find("transfer-encoding");
	if (ite != _headers.end() && ite->second.find("chunked") != std::string::npos)
		_chunkedTransfer = true;
	else if (_headers.find("content-length") == _headers.end())
		throw (BAD_REQUEST);
	_headerIsParsed = true;
}

void	Request::_parseBody()
{
	Request::listOfHeaders::const_iterator	ite;

	if (_requestStatus == COMPLETE_REQUEST)
		return ;
	if (_chunkedTransfer)
		_decodeChunks();
	else
	{
		_body = _request;
		if (_body.size() == _bodySize)
			return (_setRequestStatus(COMPLETE_REQUEST));
	}
}

void	Request::_decodeChunks()
{
	std::string		chunkSize;
	long			size;
	// size_t			pos;

	if (!_reachedEndOfChunkedBody())
	{
		return (_setRequestStatus(INCOMPLETE_REQUEST));
	}
	while (1)
	{
		size = 0;
		if (_getNextWord(chunkSize, "\r\n") == std::string::npos)
			throw (BAD_REQUEST);
		if (chunkSize.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
			throw (BAD_REQUEST);
		std::cerr << "'" << BLUE << chunkSize << "'" << RESET << NL;
		if (chunkSize == "0")
			break ;
		size = std::strtol(chunkSize.c_str(), NULL, 16);
		if (!size || size == LONG_MAX || size == LONG_MIN)
		{
			std::cerr << "'" << RED << chunkSize << "'" << RESET << NL;
			throw (BAD_REQUEST);
		}
		_body += _getNextWord(size);
		_bodySize += size;
	}
	return (_setRequestStatus(COMPLETE_REQUEST));
}

void	Request::insertUploadPath(size_t pos, const std::string& uploadPath)
{
	_body.insert(pos, uploadPath);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Request::getRequest() const
{
	return (_request);
}

t_requestStatus		Request::getRequestStatus() const
{
	return (_requestStatus);
}

t_statusCode	Request::getStatusCode() const
{
	return (_statusCode);
}

std::string		Request::getStatusCodeStr() const
{
	return (convertNbToString(_statusCode));
}

t_method	Request::getMethod() const
{
	return (_method);
}

std::string		Request::getPath() const
{
	return (_path);
}

std::string		Request::getHttpProtocol() const
{
	return (_httpProtocol);
}

Request::listOfHeaders	Request::getHeaders() const
{
	return (_headers);
}

std::string		Request::getHeader(const std::string& headerName)
{
	if (_headers.find(headerName) != _headers.end())
		return (_headers[headerName]);
	return ("");
}

size_t	Request::getBodySize() const
{
	return (_bodySize);
}

std::string	Request::getQuery() const {
	return (_query);
}

std::string		Request::getBody() const
{
	return (_body);
}

Request::listOfParsingFunctions		Request::getParsingFunct() const
{
	return (_parsingFunct);
}

bool	Request::getChunkedTransfer() const
{
	return (_chunkedTransfer);
}

std::string		Request::getHost() const
{
	return (_host);
}

int		Request::getPort() const
{
	return (_port);
}

size_t	Request::getPayloadSize() const
{
	return (_payloadSize);
}

int		Request::getFd() const {
	return (_fd);
}

std::string		Request::getMethodStr() const {
	/* Lol flemme de faire un pointeur sur fct */
	if (getMethod() == GET)
		return (std::string("GET"));
	else if (getMethod() == POST)
		return (std::string("POST"));
	else
		return (std::string("DELETE"));
}

std::string		Request::getRawRequest() const
{
	return (_raw);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

bool	Request::_headerIsSet(const std::string& headerName)
{
	listOfHeaders::const_iterator	ite;

	ite = _headers.find(headerName);
	return (ite != _headers.end());
}

bool	Request::_reachedEndOfChunkedBody()
{
	return (_request.find("0\r\n\r\n") != std::string::npos);
}

void	Request::_setRequestStatus(t_requestStatus status)
{
	_requestStatus = status;
}

size_t	Request::_getNextWord(std::string &word, std::string const& delimiter)
{
	size_t	pos;
	size_t	totalSize;

	pos = _request.find(delimiter);
	std::string nextWord(_request, 0, pos);
	totalSize = pos + delimiter.length();
	_request.erase(0, totalSize);
	word = nextWord;
	_payloadSize += totalSize;
	return (pos);
}

std::string		Request::_getNextWord(size_t sizeWord)
{
	std::string nextWord;

	nextWord = _request.substr(0, sizeWord);
	_request.erase(0, sizeWord + 2);
	_payloadSize += sizeWord + 2;
	std::cerr << "'" << GREEN << nextWord << "'" << RESET << NL;
	return (nextWord);
}

void	Request::_initParsingFunct()
{
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseMethod);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parsePath);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseHttpProtocol);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseHeaders);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_checkHeaders);
}

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Request::printRequestInfo()
{
	Request::listOfHeaders::const_iterator ite;

	#ifndef DISPLAY
		return;
	#endif

	std::cout << std::endl << BLUE << "------------ INFO REQUEST -------------" << std::endl;
	std::cout << "          method : " << GREEN << _method << std::endl;
	std::cout << BLUE << "            path : " << GREEN << _path << std::endl;
	std::cout << BLUE << "    httpProtocol : " << GREEN << _httpProtocol << std::endl;
	std::cout << BLUE << "      statusCode : " << GREEN << _statusCode << std::endl;
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
	{
		for (int i = ite->first.length(); i < 16 ; i++)
			std::cout << " ";
		std::cout << BLUE << ite->first << " : '" << GREEN << ite->second << BLUE << "'" << std::endl;
	}
	std::cout << BLUE << "            body : '" << GREEN << _body << BLUE << "'" << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	std::cout << RESET << std::endl;
}

/******************************************************************************/
/*                                 COOKIES                                    */
/******************************************************************************/

// listOfCookies&		Request::getCookies()
// {
// 	return (_cookies);
// }

const Request::listOfCookies&	Request::getCookies() const
{
	return (_cookies);
}

void	Request::_parseCookies()
{
	listOfHeaders::const_iterator	ite;

	ite = _headers.find("cookie");
	if (ite != _headers.end())
		_setCookies(ite->second);
}

void	Request::_setCookies(std::string header)
{
	std::string		name;
	std::string		value;
	size_t			pos;

	/* Limit Firefox = 150 cookies */
	for (size_t nbOfCookies = 0; nbOfCookies < 150; nbOfCookies++)
	{
		pos = header.find("=");
		if (pos == std::string::npos)
			throw(BAD_REQUEST);
		name = header.substr(0, pos);
		header.erase(0, pos + 1);
		pos = header.find("; ");
		value = header.substr(0, pos);
		header.erase(0, pos + 2);
		_addCookie(name, value);
		if (pos == std::string::npos)
			return ;
	}
	throw (PAYLOAD_TOO_LARGE);
}

void	Request::_addCookie(const std::string& name, const std::string& value)
{
	_cookies.insert(_cookies.end(), Cookie(name, value));
}

bool   Request::keepAlive() const {
	listOfHeaders::const_iterator   it;

	it = _headers.find("Connection");
	if (it != _headers.end()) {
		if (it->second == "keep-alive")
			return (true);
	}
	return (false);
}
