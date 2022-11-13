/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/11/09 18:05:39 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <cstdlib>
# include <sstream>
# include <ctime>
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <stdlib.h>

# include "StatusCode.hpp"
# include "MimeType.hpp"
# include "HttpMethod.hpp"
# include "Colors.hpp"
# include "debug.hpp"

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1
# define BUFSIZE 50000

extern bool	_triggered;
extern int	__is_child;

		/*----------   Signal   ---------*/

void			setupSignal(int state);
void			setupSigpipe(int state);
bool			getTriggeredValue();

		/*-------- String related -------*/

std::string		convertSizeToString(size_t size);
std::string		convertNbToString(int nb);
std::string		convertCharPtrToString(char* ptr);
bool			convertPort(const std::string& str, int* port);
bool			convertHttpCode(const std::string& str, int* code);
std::string		trimSpacesStr(std::string *str, const char *toTrim = " \t");
std::string		trimSpacesEndStr(std::string *str, const char *toTrim = " \t");
std::string		readFd(int fd);
char*			clone_str(const std::string& str);
std::string		generateRandomString(size_t length);
ssize_t			writeFd(int fd, const char* buf, const ssize_t len);
void			toLowerStr(std::string* str);
std::string		toLowerStr(const std::string& str);


		/*--------  File related  -------*/

bool			pathIsFile(const std::string& path);
bool			pathIsDirectory(const std::string& path);
bool			pathIsAccessible(const std::string& path);
std::string		generateCopyFilename(const std::string& dir,
				const std::string& file);
std::string		findLastFilename(const std::string& dir,
				const std::string& basename);

		/*----------   Display  ---------*/

void			DEBUG(const std::string& str);
void			displayMsg(const std::string& msg, const char* colorCode);
std::string 	extractStatusLine(const std::string& str);

		/*------------   Time   ---------*/

std::string		getFormattedDate();
std::string		getFormattedDate(std::time_t time);

#endif
