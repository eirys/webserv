/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:36:56 by etran             #+#    #+#             */
/*   Updated: 2022/11/04 11:56:44 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLINSTANCE_HPP
# define EPOLLINSTANCE_HPP

# include <errno.h>
# include <exception>
# include <string.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <string>
# include <set>

# include "Server.hpp"
# include "Client.hpp"

# define MAX_EVENT 100

class EpollInstance {
	public:
		/* -- Typedef ------------------------------------------------------ */
		typedef		std::map<int, Client*>				listOfClients;
		typedef		std::map<Server*, Block*>			serverMap;

		EpollInstance();
		virtual ~EpollInstance();

		/* -- Epoll manipulation ------------------------------------------- */
		void						startMonitoring(serverMap& servers);

		/* -- Getter ------------------------------------------------------- */
		int							getFd() const;

	private:
		/* -- Epoll manipulation ------------------------------------------- */
		void						_closeEfd();
		void						_addSocket(int socket, int flag);
		void						_editSocket(int socket, int flag);
		void						_removeSocket(int socket);

		/* -- Client list management --------------------------------------- */
		void						_addClient(serverMap::const_iterator it,
										int sock);
		void						_eraseClient(Client* client);
		void						_clearClientList();

		/* -- Server management -------------------------------------------- */
		serverMap::const_iterator	 _findServerConnection(int fd,
										const serverMap& serv) const;
		void						_monitorServers(const serverMap& serverlist);
		void						_processConnections(serverMap::const_iterator it);
		void						_handleRequest(Client* client);
		void						_handleResponse(Client* client);
		t_responseStatus			_writeResponse(Client* client);
		
		/* -- Utils -------------------------------------------------------- */
		std::string					_readRequest(Client* client);

		/* -- Data members ------------------------------------------------- */
		int							_efd;
		struct epoll_event			_events[MAX_EVENT];
		listOfClients				_clientlist;
};

#endif
