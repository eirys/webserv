#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <vector>
# include <map>
# include <string>
# include <unistd.h>
# include <iostream>
# include <utility>

# include "utils.hpp"
# include "Token.hpp"

extern HttpMethod	g_httpMethod;

# define DEFAULT_CLIENT_BODY_LIMIT 10000000
# define DEFAULT_ROOT "/"

/******************************************************************************/
/*                                 ENUMS                                      */
/******************************************************************************/

typedef enum e_context
{
	NONE		= 0,
	SERVER		= 1,
	LOCATION	= 2,
}	t_context;

/******************************************************************************/
/*                                CLASS BLOCK                                 */
/******************************************************************************/

class	Block
{
	public :
	/***********************      MEMBER TYPES      *********************/
		typedef Block*								blockPtr;
		typedef	std::vector<std::string>			listOfStrings;
		typedef std::map<std::string, blockPtr>		listOfLocations;
		typedef std::map<int, std::string>			listOfErrorPages;
		typedef std::vector<Block*>					listOfServers;
		typedef std::map<std::string, std::string>	listOfCgi;
		typedef	std::map<t_method, bool>			listOfMethods;

	private :
	/**********************     MEMBER VARIABLES     ********************/


					/*-----   Server block only   -----*/
		t_context							_context;
		listOfStrings						_serverNames;
		int									_port;
		std::string							_host;
		listOfServers						_virtualHosts;
		std::string							_virtualServerName;

					/*---- Server / Location block ----*/
		std::string							_root;
		listOfStrings						_indexes;
		bool								_autoindex;
		size_t								_clientBodyLimit;
		listOfCgi							_cgi;
		listOfErrorPages					_errorPages;
		int									_redirectCode;
		std::string							_redirectUri;
		std::vector<Token::tokenType>		_setDirectives;

					/*-----  Location block only  -----*/
		listOfLocations						_locations;
		listOfLocations::const_iterator		_currentLocation;
		listOfMethods						_methods;
		std::string							_uploadPath;

	public :
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Block();
		Block(const Block& other);
		~Block();

						/*----- Operator overloads ----*/
		Block&								operator=(const Block& other);
		bool								operator==(Block const& otherServer);

						/*------- Set directives -----*/
		void								setDirective(Token::tokenType directive);
		bool								directiveIsSet(Token::tokenType directive);

						/*-------  Server_name  ------*/
		void								setName(const std::string& name);
		listOfStrings						getServerNames() const;	
		void								setVirtualServerName(const std::string& name);
		std::string							getServerName() const;	

						/*-------    Listen    -------*/
		void								setPort(int port);
		int									getPort() const;
		void								setHost(const std::string& host);
		const std::string&					getHost() const;

						/*-------     Root     -------*/
		void								setRoot(const std::string& root);
		const std::string&					getRoot() const;

						/*-------     Index    -------*/
		void								setIndex(const std::string& index);
		listOfStrings						getIndexes() const;

						/*-------   Autoindex  -------*/
		void								setAutoindex(bool value);
		bool								getAutoindex() const;

						/*--- Client_max_body_size ---*/
		void								setClientBodyLimit(size_t size);
		unsigned long						getClientBodyLimit() const;

						/*-------      CGI     -------*/
		void								setCgi(const std::string& extension, const std::string& path);
		const listOfCgi&					getCgi() const;
		std::string							findCgi(const std::string& extension);

						/*-------  Error_page  -------*/
		void								setErrorPage(int code, const std::string& page);
		listOfErrorPages					getErrorPages() const;
		std::string							getErrorPage(int statusCode) const;

						/*-------    Redirect  -------*/
		void								setRedirection(int code, const std::string& uri);
		const std::string&					getRedirectUri() const;
		int									getRedirectCode() const;
		bool								redirectDirective();

						/*-------  Upload_path  -------*/
		void								setUploadPath(const std::string& path);
		const std::string&					getUploadPath() const;
		bool								uploadPathDirective();

						/*------- Allowed_method ------*/
		void								initMethods(bool value);
		void								setMethod(t_method method, bool value);
		void								setMethod(const std::string& str, bool value);
		bool								isAllowedMethod(t_method method);
		listOfMethods						getMethods() const;

						/*-------    Location   ------*/
		bool								insertLocation(const std::string& path, blockPtr newLocation);
		int									getNbOfLocations() const;
		listOfLocations::const_iterator		getCurrentLocation() const;
		listOfLocations						getLocations() const;

						/*-------    Context    ------*/
		void								setContext(t_context context);
		t_context							getContext() const;

						/*------ Virtual hosts -----*/
		void								setVirtualHost(blockPtr server);
		listOfServers						getVirtualHosts() const;

						/*-------     Utils    -------*/
		bool								isServerBlock();
		bool								isLocationBlock();
		void								completeLocationDirectives(const Block& server);
		Block*								getMatchingBlock(const std::string& path, std::string* locationPath);

						/*-------    Display    ------*/
		void								displayListOfStrings(const listOfStrings& list);
		void								displayBlockDirectives(t_context context);
		void								displayParams(int num);
		void								displayServerNames();

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------     Utils    -------*/
		void								_deleteLocations();
		void								_deleteVirtualHosts();

};


#endif
