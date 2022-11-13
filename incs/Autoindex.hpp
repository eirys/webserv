#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include <iostream>
# include <ctime>

# include <stdio.h>
# include <unistd.h>
# include <sys/stat.h>
# include <dirent.h>

# include "utils.hpp"

/******************************************************************************/
/*                               CLASS AUTOINDEX                              */
/******************************************************************************/

class Autoindex
{
 	private:
	/**********************     MEMBER VARIABLES     ********************/
        std::string     _directoryPath;
        std::string     _indexPage;

    public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
        Autoindex(const std::string& path);
        Autoindex(const Autoindex& other);
        ~Autoindex();
        Autoindex&      operator=(const Autoindex& other);

						/*-------    Getter   ------*/
        std::string     getIndexPage() const;
        std::string     getDirectoryPath() const;

    private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*----- Generate HTML ------*/
        void            _generateIndexPage();
        std::string	    _generateHtmlHeader();
        std::string	    _generateHtmlFooter();
		std::string		_generateHtmlLink(const unsigned char fileType, const std::string& fileName);
		std::string		_getFileLink(const unsigned char fileType, std::string fileName);

						/*-------    Utils   ------*/
		void			_formatCell(std::string* data);
		std::string		_getFileSize(struct stat s);
		std::string		_getFileModificationTime(struct stat s);
};

#endif
