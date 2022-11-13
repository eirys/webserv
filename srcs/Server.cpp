/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:12 by etran             #+#    #+#             */
/*   Updated: 2022/11/02 11:17:36 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "Server.hpp"

Server::Server(Block* x, char* const* env) :
	_socket(socket(PF_INET, SOCK_STREAM, 0)),
	_ip(x->getHost()),
	 _env(env) {
		_setMetaVar(x);
		_socket.setToReusable();
		memset(&_addr, 0, sizeof(_addr));
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(x->getPort());
		_addr.sin_addr.s_addr = inet_addr(_ip.c_str());

		if (bind(getSocket(), reinterpret_cast<struct sockaddr*>(&_addr),
				sizeof(_addr)) < 0)
			throw std::runtime_error("Server() bind error");
		_socket.unlockSocket();
		_socket.listenSocket();

		_displayServer(); /* DEBUG */
	}

Server::~Server() {
	DEBUG("Server Block destructor");
}

/* PUBLIC =================================================================== */

// Getter ----------------------------------------

int Server::getPort() const {
	return (ntohs(_addr.sin_port));
}

const std::string& Server::getHost() const {
	return (_ip);
}

int Server::getSocket() const {
	return (_socket.getFd());
}

const struct sockaddr_in& Server::getAddr() const {
	return (_addr);
}

const Env& Server::getEnv() const {
	return (_env);
}

// Boolean operator ------------------------------

bool	Server::operator==(int fd) const {
	return (getSocket() == fd);
}

bool	Server::operator!=(int fd) const {
	return (getSocket() != fd);
}

bool	Server::operator==(const Server& rhs) const {
	return (getSocket() == rhs.getSocket());
}

bool	Server::operator!=(const Server& rhs) const {
	return (getSocket() != rhs.getSocket());
}

bool	Server::operator<(const Server& rhs) const {
	return (getSocket() < rhs.getSocket());
}

bool	Server::operator>(const Server& rhs) const {
	return (getSocket() > rhs.getSocket());
}

bool	Server::operator<=(const Server& rhs) const {
	return (getSocket() <= rhs.getSocket());
}

bool	Server::operator>=(const Server& rhs) const {
	return (getSocket() >= rhs.getSocket());
}

/* PRIVATE ================================================================== */

// Utils -----------------------------------------

void Server::_setMetaVar(const Block* block) {
	_env.addParam("GATEWAY_INTERFACE", "CGI/1.1");
	_env.addParam("SERVER_NAME", block->getServerName());
	_env.addParam("SERVER_SOFTWARE", "webserv");
	_env.addParam("AUTH_TYPE", "");
	_env.addParam("DOCUMENT_ROOT", block->getRoot());
	/* Add other Metavar ? */
}

// Debug -----------------------------------------

void Server::_displayServer() const {
	displayMsg("   Server is listening on " + getHost() + ":" + convertNbToString(getPort()), GREY);
	#ifndef DISPLAY
		return;
	#endif
	DEBUG(" ** New server created ** ");
	std::cout	<< "== Connection infos ==" << NL
				<< "fd: " << getSocket() << NL
				<< "ip: " << getHost() << NL
				<< "port: " << getPort() << NL;
}

// Bonus -----------------------------------------

Session*	Server::lookupSession(const listOfCookies& requestCookies)
{
	return (_sessionHandler.lookupSession(requestCookies));
}
