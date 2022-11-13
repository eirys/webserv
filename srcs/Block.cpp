#include "Block.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Block::Block():
	_context(NONE),
	_serverNames(0),
	_port(8000),
	_host("0.0.0.0"),
	_root(DEFAULT_ROOT),
	_indexes(0),
	_autoindex(false),
	_clientBodyLimit(DEFAULT_CLIENT_BODY_LIMIT),
	_redirectCode(0),
	_redirectUri(""),
	_uploadPath("")
{
	initMethods(true);
	/* if I'm root user, I use port 80 */
	if (!getuid())
		setPort(80);
}

Block::Block(const Block& other)
{
	*this = other;
}

Block::~Block()
{
	if (isServerBlock())
	{
		_deleteLocations();
		_deleteVirtualHosts();
	}
}

/******************************************************************************/
/*                           OPERATOR OVERLOAD                                */
/******************************************************************************/

Block&	Block::operator=(const Block& other)
{
	if (this != &other)
	{
		_context = other.getContext();
		_serverNames = other.getServerNames();
		_port = other.getPort();
		_host = other.getHost();
		_virtualHosts = other.getVirtualHosts();
		_root = other.getRoot();
		_indexes = other.getIndexes();
		_autoindex = other.getAutoindex();
		_clientBodyLimit = other.getClientBodyLimit();
		_cgi = other.getCgi();
		_errorPages = other.getErrorPages();
		_redirectCode = other.getRedirectCode();
		_redirectUri = other.getRedirectUri();
		_locations = other.getLocations();
		_currentLocation = other.getCurrentLocation();
		_methods = other.getMethods();
		_uploadPath = other.getUploadPath();
	}
	return (*this);
}

bool	Block::operator==(Block const& otherServer)
{
	return (this->getContext() == SERVER && otherServer.getContext() == SERVER
		&& this->getHost() == otherServer.getHost()
		&& this->getPort() == otherServer.getPort());
}

void	Block::setDirective(Token::tokenType directive)
{
	_setDirectives.insert(_setDirectives.end(), directive);
}

/* Check if the directive was already set */
bool	Block::directiveIsSet(Token::tokenType directive)
{
	std::vector<Token::tokenType>::const_iterator	ite;

	/* Some directives can be called multiple times (location, error_page, cgi) */
	if (directive == KEYWORD_LOCATION || directive == KEYWORD_ERROR_PAGE
		|| directive == KEYWORD_CGI)
		return (false);
	for (ite = _setDirectives.begin(); ite != _setDirectives.end(); ite++)
	{
		if (*ite == directive)
			return (true);
	}
	return (false);
}


/******************************************************************************/
/*                              SERVER_NAME                                   */
/******************************************************************************/

void	Block::setName(const std::string& name)
{
	_serverNames.insert(_serverNames.end(), name);
}

Block::listOfStrings	Block::getServerNames() const
{
	return (_serverNames);
}

std::string		Block::getServerName() const
{
	if (!_virtualServerName.empty())
		return (_virtualServerName);
	if (!_serverNames.empty())
		return (_serverNames[0]);
	return (_host);
}

void	Block::setVirtualServerName(const std::string& name)
{
	_virtualServerName = name;
}

/******************************************************************************/
/*                                 LISTEN                                     */
/******************************************************************************/

void	Block::setPort(int port)
{
	_port = port;
}

int		Block::getPort() const
{
	return (_port);
}

void	Block::setHost(const std::string &host)
{
	_host = host;
}

const std::string&	Block::getHost() const
{
	return (_host);
}

/******************************************************************************/
/*                                  ROOT                                      */
/******************************************************************************/

void	Block::setRoot(const std::string &root)
{
	_root = root;
}

const std::string&	Block::getRoot() const
{
	return (_root);
}

/******************************************************************************/
/*                                  INDEX                                     */
/******************************************************************************/

void	Block::setIndex(const std::string &index)
{
	_indexes.insert(_indexes.end(), index);
}

Block::listOfStrings	Block::getIndexes() const
{
	return (_indexes);
}

/******************************************************************************/
/*                                AUTOINDEX                                   */
/******************************************************************************/

void	Block::setAutoindex(bool value)
{
	_autoindex = value;
}

bool	Block::getAutoindex() const
{
	return (_autoindex);
}

/******************************************************************************/
/*                            CLIENT_MAX_BODY_SIZE                            */
/******************************************************************************/

void	Block::setClientBodyLimit(size_t size)
{
	_clientBodyLimit = size;
}

size_t	Block::getClientBodyLimit() const
{
	return (_clientBodyLimit);
}

/******************************************************************************/
/*                                   CGI                                      */
/******************************************************************************/

void	Block::setCgi(const std::string& extension, const std::string& path)
{
	_cgi[extension] = path;
}

const Block::listOfCgi&		Block::getCgi() const
{
	return (_cgi);
}

std::string		Block::findCgi(const std::string& extension)
{
	if (_cgi.find(extension) != _cgi.end())
		return (_cgi[extension]);
	return ("");
}

/******************************************************************************/
/*                                ERROR_PAGE                                  */
/******************************************************************************/

void	Block::setErrorPage(int code, const std::string& page)
{
	_errorPages[code] = page;
}

Block::listOfErrorPages		Block::getErrorPages() const
{
	return (_errorPages);
}

std::string		Block::getErrorPage(int statusCode) const
{
	Block::listOfErrorPages::const_iterator	ite;
	std::string								errorPage;

	errorPage = "";
	ite = _errorPages.find(statusCode);
	if (ite != _errorPages.end())
		errorPage = ite->second;
	return (errorPage);
}

/******************************************************************************/
/*                                 REDIRECT                                   */
/******************************************************************************/

void	Block::setRedirection(int code, const std::string& uri)
{
	_redirectCode = code;
	_redirectUri = uri;
}

const std::string&	Block::getRedirectUri() const
{
	return (_redirectUri);
}

int		Block::getRedirectCode() const
{
	return (_redirectCode);
}

bool	Block::redirectDirective()
{
	return (!_redirectUri.empty() && _redirectCode != 0);
}

/******************************************************************************/
/*                              UPLOAD_PATH                                   */
/******************************************************************************/

void	Block::setUploadPath(const std::string& path)
{
	_uploadPath = path;
}

const std::string	&Block::getUploadPath() const
{
	return (_uploadPath);
}

bool	Block::uploadPathDirective()
{
	return (!_uploadPath.empty());
}

/******************************************************************************/
/*                             ALLOWED_METHOD                                 */
/******************************************************************************/

void	Block::setMethod(t_method method, bool value)
{
	_methods[method] = value;
}

void	Block::setMethod(const std::string& str, bool value)
{
	t_method	method;

	method = g_httpMethod.getMethod(str);
	_methods[method] = value;
}

bool	Block::isAllowedMethod(t_method method)
{
	return (_methods[method]);
}

Block::listOfMethods	Block::getMethods() const
{
	return (_methods);
}

void	Block::initMethods(bool value)
{
	setMethod(GET, value);
	setMethod(POST, value);
	setMethod(DELETE, value);
}
/******************************************************************************/
/*                                LOCATION                                    */
/******************************************************************************/

bool	Block::insertLocation(const std::string& path, blockPtr newLocation)
{
	std::pair<listOfLocations::const_iterator, bool>	ret;

	ret = _locations.insert(std::make_pair(path, newLocation));
	_currentLocation = ret.first;
	return (ret.second);
}

Block::listOfLocations::const_iterator	Block::getCurrentLocation() const
{
	return (_currentLocation);
}

Block::listOfLocations	Block::getLocations() const
{
	return (_locations);
}

int		Block::getNbOfLocations() const
{
	int									count;
	listOfLocations::const_iterator		ite;

	count = 0;
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
		count++;
	return (count);
}

void	Block::_deleteLocations()
{
	for (_currentLocation = _locations.begin(); _currentLocation != _locations.end(); _currentLocation++)
	{
		delete (_currentLocation->second);
	}
}

/******************************************************************************/
/*                                 CONTEXT                                    */
/******************************************************************************/

t_context		Block::getContext() const
{
	return (_context);
}

void	Block::setContext(t_context context)
{
	_context = context;
}

/******************************************************************************/
/*                               VIRTUAL HOST                                 */
/******************************************************************************/

void	Block::setVirtualHost(blockPtr server)
{
	blockPtr	virtualHost;

	virtualHost = new Block(*server);
	_virtualHosts.push_back(virtualHost);
	delete server;
}

Block::listOfServers	Block::getVirtualHosts() const
{
	return (_virtualHosts);
}

void	Block::_deleteVirtualHosts()
{
	listOfServers::const_iterator	ite;

	for (ite = _virtualHosts.begin(); ite != _virtualHosts.end(); ite++)
	{
		delete (*ite);
	}
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

void	Block::completeLocationDirectives(const Block& server)
{
	if (this == &server)
		return ;
	if (_root.empty())
		_root = server.getRoot();
	if (_indexes.empty())
		_indexes = server.getIndexes();
	if (_clientBodyLimit == DEFAULT_CLIENT_BODY_LIMIT)
		_clientBodyLimit = server.getClientBodyLimit();
	if (_cgi.empty())
		_cgi = server.getCgi();
	if (_errorPages.empty())
		_errorPages = server.getErrorPages();
	if (!_redirectCode)
		_redirectCode = server.getRedirectCode();
	if (_redirectUri.empty())
		_redirectUri = server.getRedirectUri();
}

Block*		Block::getMatchingBlock(const std::string& path, std::string* locationPath)
{
	size_t		pos;
	std::string	prefix;

	pos = path.find("/", 1);
	prefix = path.substr(0, pos);
	_currentLocation = _locations.find(prefix);
	if (_currentLocation != _locations.end())
	{
		*locationPath = _currentLocation->first;
		return (_currentLocation->second);
	}
	if (path[0] == '/')
	{
		_currentLocation = _locations.find("/");
		if (_currentLocation != _locations.end())
		{
			*locationPath = _currentLocation->first;
			return (_currentLocation->second);
		}
	}
	return (this);
}

bool	Block::isServerBlock()
{
	return (_context == SERVER);
}

bool	Block::isLocationBlock()
{
	return (_context == LOCATION);
}

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Block::displayParams(int num)
{
	listOfLocations::const_iterator	ite;

	std::cout << std::endl << " SERVER " << num << ": " << std::endl;
	std::cout << "  ‣ Names: ";
	displayListOfStrings(_serverNames);
	std::cout << "  ‣ Listen: " << getHost() << ":" << getPort() << std::endl;
	displayBlockDirectives(SERVER);
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
	{
		std::cout << std::endl << "     Location " << ite->first << ": " << std::endl;
		ite->second->displayBlockDirectives(LOCATION);
	}
}

void	Block::displayBlockDirectives(t_context context)
{
	std::string indent;

	indent = (context == SERVER) ? "  " : "     "; 
	std::cout << indent << "‣ Root: " << getRoot() << std::endl;
	std::cout << indent << "‣ Index: ";
	displayListOfStrings(_indexes);
	std::cout << indent << "‣ Autoindex: " << getAutoindex() << std::endl;
	std::cout << indent << "‣ Body limit: " << getClientBodyLimit() << std::endl;
	std::cout << indent << "‣ Upload path: " << getUploadPath() << std::endl;
	std::cout << indent << "‣ Redirection: " << getRedirectCode() << " " << getRedirectUri() << std::endl;
}

void	Block::displayListOfStrings(const listOfStrings& list)
{
	listOfStrings::const_iterator	ite;

	for (ite = list.begin(); ite != list.end(); ite++)
		std::cout << *ite << " ";
	std::cout << std::endl;
}
