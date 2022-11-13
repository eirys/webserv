#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <cstdlib>
# include <climits>
# include <sstream>

#include "Block.hpp"
#include "utils.hpp"
# include "Session.hpp"

# define UNDEFINED_PORT -1

extern HttpMethod	g_httpMethod;

typedef enum t_requestStatus
{
	INCOMPLETE_REQUEST	= 0,
	COMPLETE_REQUEST	= 1,
	INVALID_REQUEST		= 2,
}	t_requestStatus;

class   Request
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef void (Request::*parsingFunction)();
		typedef std::vector<parsingFunction>		listOfParsingFunctions;
		typedef std::map<std::string, std::string>	listOfHeaders;
		typedef Session::listOfCookies				listOfCookies;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string					_request;
		t_requestStatus				_requestStatus;
		t_statusCode				_statusCode;
		t_method					_method;
		std::string					_path;
		std::string					_httpProtocol;
		listOfHeaders				_headers;
		size_t						_bodySize;
		std::string					_body;
		listOfParsingFunctions		_parsingFunct;
		bool						_chunkedTransfer;
		std::string					_host;
		int							_port;
		size_t						_payloadSize;
		int							_fd;
		std::string					_query;
		listOfCookies				_cookies;
		std::string					_raw;
		bool						_headerIsParsed;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Request();
		Request(const std::string& buffer, int clientfd);
        Request(const Request& other);
        ~Request();
        Request&    				operator=(const Request& other);

						/*------   Parsing  ------*/
		t_requestStatus				parseRequest();
		void						completeRequest(const std::string& buffer);

						/*------   Getter  ------*/
		std::string					getRequest() const;
		t_method					getMethod() const;
		std::string					getPath() const;
		std::string					getHttpProtocol() const;
		listOfHeaders				getHeaders() const;
		std::string					getHeader(const std::string& headerName);
		size_t						getBodySize() const;
		std::string					getBody() const;
		t_statusCode				getStatusCode() const;
		std::string					getStatusCodeStr() const;
		t_requestStatus				getRequestStatus() const;
		listOfParsingFunctions		getParsingFunct() const;
		bool						getChunkedTransfer() const;
		std::string					getHost() const;
		int							getPort() const;
		size_t						getPayloadSize() const;
		int							getFd() const;
		std::string					getMethodStr() const; // Can be replaced by global
		std::string					getQuery() const;
		std::string					getRawRequest() const;
		const listOfCookies&		getCookies() const;

						/*------   Display  ------*/
		void						printRequestInfo();
		bool						keepAlive() const;

		void						insertUploadPath(size_t pos, const std::string& uploadPath);

	private:
	/**********************  PRIVATE MEMBER FUNCTIONS  ******************/

		void						_initVariables();

						/*------   Parsing  ------*/
		void						_runParsingFunctions();
		void						_parseMethod();
		void						_parsePath();
		void						_parseHttpProtocol();
		void						_parseHeaders();
		void						_checkHeaders();
		void						_checkContentLength();
		void						_parseBody();
		bool						_parseHostHeader();
		void						_decodeChunks();

						/*------   Utils  ------*/
		void						_initParsingFunct();
		void						_setRequestStatus(t_requestStatus status);
		size_t						_getNextWord(std::string& word, std::string const& delimiter);
		std::string					_getNextWord(size_t sizeWord);
		std::string					_toLowerStr(std::string* str);
		bool						_reachedEndOfChunkedBody();
		bool						_headerIsSet(const std::string& headerName);
		void						_checkSizeBody();

						/*------   Cookies  ------*/
		void						_parseCookies();
		void						_setCookies(std::string header);
		void						_addCookie(const std::string& name, const std::string& value);
};

#endif
