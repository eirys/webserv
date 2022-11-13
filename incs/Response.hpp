/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 19:15:58 by etran             #+#    #+#             */
/*   Updated: 2022/11/09 23:23:30 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>
# include <cstdio>
# include <cerrno>
# include <iostream>
# include <fstream>

# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# define WEBSERV_VERSION	"Cutie Webserv 1.0"

# include "Request.hpp"
# include "MimeType.hpp"
# include "Autoindex.hpp"
# include "Env.hpp"
# include "CgiHandler.hpp"

extern StatusCode	g_statusCode;
extern MimeType		g_mimeType;

typedef enum t_responseStatus
{
	INCOMPLETE_RESPONSE	= 0,
	COMPLETE_RESPONSE	= 1,
	INVALID_RESPONSE	= 2,
}	t_responseStatus;

class   Response
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef void (Response::*httpMethod)();
		typedef std::map<t_method, httpMethod>		listOfHttpMethodsFunct;
		typedef Request::listOfHeaders				listOfHeaders;
		typedef Block::listOfStrings				listOfStrings;
		typedef Session::listOfCookies				listOfCookies;
		typedef Session::listOfPurchases			listOfPurchases;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		Block*							_server;
		Block*							_matchingBlock;
		Request*						_request;
		std::string						_response;
		t_statusCode					_statusCode;
		t_method						_method;
		listOfHeaders					_headers;
		std::string						_body;
		listOfHttpMethodsFunct			_httpMethods;
		std::string						_locationPath;
		int								_fd;
		std::string						_builtPath;
		std::string						_uploadPath;

		Env*							_env; // Shares same env than client
		std::string						_cgiscript;
		std::string						_cgiextra;
		std::string						_cgiquery;
		std::string						_cgipath;
		Session*						_session;
		std::string						_msg;
		std::vector<std::string>		_cgiFilenames;
		std::string						_referer;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Response(Block* server, Request* request, Env& env, Session* session);
        Response(const Response& other);
        ~Response();
        Response&    					operator=(const Response& other);

						/*-------  Generate  ------*/
		void							generateResponse();

						/*-------   Setter  ------*/
		void							setStatusCode(t_statusCode status);
		void							setStatusCode(int status);

						/*-------   Getter  ------*/
		Block*							getServer() const;
		Block*							getMatchingBlock() const;
		Request*						getRequest() const;
		std::string						getResponse() const;
		t_statusCode					getStatusCode() const;
		std::string						getStatusCodeStr() const;
		t_method						getMethod() const;
		listOfHeaders					getHeaders() const;
		std::string						getBody() const;
		listOfHttpMethodsFunct			getHttpMethods() const;
		std::string						getLocationPath() const;
		int								getFd() const;
		std::string						getBuiltPath() const;

		std::string						getCgiProgram() const;
		std::string						getCgiName() const;
		std::string						getCgiExtra() const;
		std::string						getCgiQuery() const;
		Session*						getSession() const;
		Env*							getEnv() const;
		std::string						getMsgToDisplay() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
						/*-------   Init    ------*/
		void							_initHttpMethods();

						/*-------  Generate  ------*/
        std::string						_generateErrorPage();
		void							_processMethod();
		void							_fillResponseLine();
		void							_fillHeaders();
		void							_fillHeader(const std::string& name, const std::string& value);
		void							_fillErrorBody();

						/*------  Get Method  -----*/
		void							_runGetMethod();
		void							_readFileContent(std::string& path);

						/*------  Post Method -----*/
		void							_runPostMethod();
		void							_writeFileContent(const std::string& path, const std::string& content);
		void							_handleUploadFile();
		void							_handleCgi();
		void							_handleMultipartContent(std::string body);
		void							_handleMultipartContentCgi(std::string body);
		//std::string						_getBoundary(std::string contentType);
		bool							_getBoundary(std::string contentType,
										std::string& boundary);
		// std::string						_getField(std::string contentDisposition, const std::string& field);
		size_t							_getField(std::string contentDisposition, const std::string& field,
											std::string* name);
		void							_parseContent(std::string body, const std::string& boundary);

						/*-----  Delete Method ----*/
		void							_runDeleteMethod();
		bool							_deletePurchase(const std::string& uri);
		bool							_deletePurchaseImage(const std::string& uri);

						/*-----      Query    ----*/
		void							_parseQuery();

						/*-------     Path  ------*/
		void							_buildPath();
		void							_checkPath();
		std::string 					_getPathDir(const std::string& path);
		void							_parseRequestReferer();
		void							_handleSlash(std::string* path, const std::string& uri);
		void							_handleDirectoryPath(std::string* path);
		bool							_hasUploadPathDirective();
		bool							_searchOfIndexPage(const listOfStrings& indexes, std::string* path);
		bool							_foundIndexPage(DIR* dir, const std::string& indexPage);
		void							_generateAutoindex(const std::string& path);
		void							_checkUploadPath();

						/*-----   Redirection  ----*/
		void							_handleRedirection();

						/*--------   Error  -------*/
		void							_throwErrorMsg(t_statusCode errorCode, const std::string& message);
		void							_throwErrorMsg(const std::string& message);
		t_statusCode					_findErrorCode();

						/*-------  Headers   ------*/
		std::string						_getDateHeader();
		std::string						_getContentTypeHeader();
		std::string						_getConnectionHeader();

						/*-------   Utils    ------*/
		bool							_requestIsValid();
		void							_checkBodyLimit();
		bool							_contentTypeIsMultipart();
		bool							_contentTypeIsUrlencoded();
		bool							_isCgi(const std::string& path);
		void							_checkSizeFile(const std::string& path);

						/*-------    Cgi     ------*/
		size_t							_parsePosCgiExtension(std::string path);
		void							_parseCgiUrl(size_t pos);
		void							_fillCgiMetavariables();
		std::string						_translateCgiName() const;
		void							_parseCgiBody();
		void							_parseCgiStatusLine();
		size_t							_getNextWord(std::string& body, std::string &word, std::string const& delimiter);

						/*-------  Test page ------*/
		std::string						_generateFormAcceptPage();
		std::string						_generateFormOrderPage();
		std::string						_generateGalleryPage();

						/*-----  Bonus Cookies ----*/
		void							_fillCookieHeader();
};

#endif
