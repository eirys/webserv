/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:34:52 by etran             #+#    #+#             */
/*   Updated: 2022/11/04 11:46:27 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "TcpSocket.hpp"

TcpSocket::TcpSocket() :
	_sockfd(-1),
	_autoclosing(true) {}

TcpSocket::TcpSocket(int val, bool autoclose) :
	_sockfd(val),
	_autoclosing(autoclose) {
		if (val < 0)
			throw std::runtime_error("socket open error");
	}

TcpSocket::~TcpSocket() {
	_closeFd();
}

/* PUBLIC ================================================================== */

// Socket manipulation --------------------------

void TcpSocket::setToReusable() {
	int	_yes = 1;
	if (setsockopt(getFd(), SOL_SOCKET,
		SO_REUSEADDR | SO_REUSEPORT, &_yes, sizeof(_yes)) < 0)
		throw std::runtime_error("setsockopt error");
}

void TcpSocket::unlockSocket() {
	if (fcntl(getFd(), F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl error");
}

void TcpSocket::listenSocket() {
	if (listen(getFd(), LISTEN_VAL) < 0)
		throw std::runtime_error("listen error");
}

// Getter ---------------------------------------

int TcpSocket::getFd() const {
	return (_sockfd);
}

/* PRIVATE ================================================================= */

void TcpSocket::_closeFd() {
	if (getFd() != -1 && _autoclosing)
		if (close(_sockfd) < 0)
			throw std::runtime_error("close socket error");
}

/* NON-MEMBER ============================================================== */

// Operator overload -----------------------------

bool	operator<(const TcpSocket& lhs, const TcpSocket& rhs)
{
	return (lhs.getFd() < rhs.getFd());
}
