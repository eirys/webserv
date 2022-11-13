#ifndef MIME_TYPE_HPP
# define MIME_TYPE_HPP

# include <string>
# include <map>

class MimeType
{
	private:
		std::map<std::string, std::string>	_mimeTypes;

	public:
		MimeType();
		~MimeType();
		std::string		operator[](const std::string &ext);

	private:
		void			_initMimeTypesMap();
};

#endif