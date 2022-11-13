#include "Client.hpp"

/* All the operations :
    - accepting connections from clients
    - reading
    - writing data
are done in a non-blocking manner. */

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Client::Client(serverMapNode server, int sockfd, const Env& env):
	_sockfd(sockfd),
    _runningServer(server),
    _request(0),
    _response(0),
	_env(env),
	_toSend(0),
	_totalSent(0) {}

Client::Client(Client const& other)
{
    *this = other;
}

Client::~Client()
{
    clear();
	DEBUG("client destroyed: " + convertNbToString(getFd()));
}

Client&     Client::operator=(const Client& other)
{
    if (this != &other)
    {
		_sockfd = other.getFd();
        _runningServer = other.getRunningServer();
        _request = other.getRequest();
        _response = other.getResponse();
		_env = other.getEnv();
		_toSend = other.getToSend();
		_totalSent = other.getTotalSent();
		_responseStr = other.getResponseStr();
    }
    return (*this);
}

/******************************************************************************/
/*                                 REQUEST                                    */
/******************************************************************************/

t_requestStatus     Client::parseRequest(const std::string& buffer)
{
    t_requestStatus requestStatus;

    if (!_request)
		_request = new Request(buffer, getFd());
    else
        _request->completeRequest(buffer);
    requestStatus = _request->parseRequest();
    return (requestStatus);
}

/******************************************************************************/
/*                                 RESPONSE                                   */
/******************************************************************************/

void	Client::generateResponse()
{
	Session* session;

	if (!_responseStr.empty())
		return ;
	session = _runningServer.first->lookupSession(_request->getCookies());
	_response = new Response(_selectVirtualServer(), _request, _env, session);
	_response->generateResponse();
	_responseStr = _response->getResponse();
	_toSend = _responseStr.size();
	displayConnectionInfos();
	delete _response;
	_response = NULL;
}

bool	Client::_matchingServerName(
	const listOfStrings& serverNames, int listeningPort, Block* currentServer)
{
	listOfStrings::const_iterator	currentName;
	int								requestedPort;
	
	requestedPort = _request->getPort();
    for (currentName = serverNames.begin(); currentName != serverNames.end(); currentName++)
	{
		if (_request->getHost() == *currentName
			&& (requestedPort == UNDEFINED_PORT || requestedPort == listeningPort))
		{
			/* Configure virtual server name */
			currentServer->setVirtualServerName(*currentName);
			return (true);
		}
	}
    return (false);
}

Block*  Client::_selectVirtualServer()
{
	listOfServers				virtualHosts;
	listOfServers::iterator		currentServer;

	virtualHosts = _runningServer.second->getVirtualHosts();
    if (virtualHosts.empty())
		return (_runningServer.second);
	for (currentServer = virtualHosts.begin(); currentServer != virtualHosts.end(); currentServer++)
	{
		if (_matchingServerName((*currentServer)->getServerNames(), (*currentServer)->getPort(), *currentServer))
			return (*currentServer);
	}
	return (_runningServer.second);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Client::serverMapNode	Client::getRunningServer() const
{
    return (_runningServer);
}

Request*    Client::getRequest() const
{
    return (_request);
}

Response*   Client::getResponse() const
{
    return (_response);
}

int	Client::getFd() const {
	return (_sockfd);
}

const Env&	Client::getEnv() const {
	return (_env);
}

Env&	Client::getEnv() {
	return (_env);
}

std::string		Client::getResponseStr() const
{
	return (_responseStr);
}

size_t	Client::getTotalSent() const
{
	return (_totalSent);
}

size_t	Client::getToSend() const
{
	return (_toSend);
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

void	Client::clear()
{
	if (_request)
	{
		delete _request;
		_request = NULL;
	}
	if (_response)
	{
		delete _response;
		_response = NULL;
	}
}

void	Client::displayConnectionInfos()
{
	int			statusCode;
	std::string	responseMsg;

	statusCode = _response->getStatusCode();
	responseMsg = _response->getMsgToDisplay();
	if (responseMsg.empty() && _response->getStatusCode() == OK)
	{
		std::cout << GREY << "  [" << _sockfd << "]\t[200 OK]   [";
		std::cout << extractStatusLine(_request->getRawRequest()) << "]";
		std::cout << RESET << std::endl;
		return ;
	}
	std::string connection = "\n Connection accepted on socket " + convertNbToString(_sockfd);
	std::cout << std::endl;
	displayMsg(connection, BLUE_B);
	std::cout << LIGHT_BLUE << "   > " << extractStatusLine(_request->getRawRequest()) << std::endl;
	if (statusCode < 400)
		std::cout << LIGHT_GREEN;
	else
		std::cout << RED;
	std::cout << "   < " << extractStatusLine(_responseStr) << std::endl;
	if (!responseMsg.empty() && statusCode < 400)
	{
		std::cout << WHITE << responseMsg << std::endl;
	}
	std::cout << RESET << std::endl;
}

void	Client::eraseResponsePart(size_t size)
{
	_responseStr.erase(0, size);
}

bool	Client::responseSentIsComplete() const
{
	return (_totalSent == _toSend);
}

void	Client::completeTotalSent(size_t count)
{
	_totalSent += count;
}
