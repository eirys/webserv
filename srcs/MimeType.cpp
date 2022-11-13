#include "MimeType.hpp"

MimeType::MimeType()
{
	_initMimeTypesMap();
}

MimeType::~MimeType() {}

std::string		MimeType::operator[](const std::string &ext)
{
	std::map<std::string, std::string>::iterator	ite;

	ite = _mimeTypes.find(ext);
	if (ite != _mimeTypes.end())
	{
		if (ite->second == "text/html")
			return (ite->second + "; charset=utf-8");
		return (ite->second);
	}
	return (_mimeTypes[".bin"]);
}

/* This table lists important MIME types for the Web: */
void	MimeType::_initMimeTypesMap()
{
	_mimeTypes[".aac"] = "audio/aac";
	_mimeTypes[".abw"] = "application/x-abiword";
	_mimeTypes[".arc"] = "application/x-freearc";
	_mimeTypes[".avif"] = "image/avif";
	_mimeTypes[".avi"] = "video/x-msvideo";
	_mimeTypes[".azw"] = "application/vnd.amazon.ebook";
	_mimeTypes[".bin"] = "application/octet-stream"; /* default type */
	_mimeTypes[".bmp"] = "image/bmp";
	_mimeTypes[".bz"] = "application/x-bzip";
	_mimeTypes[".bz2"] = "application/x-bzip2";
	_mimeTypes[".cda"] = "application/x-cdf";
	_mimeTypes[".csh"] = "application/x-csh";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".csv"] = "text/csv";
	_mimeTypes[".doc"] = "application/msword";
	_mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	_mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	_mimeTypes[".epub"] = "application/epub+zip";
	_mimeTypes[".gz"] = "application/gzip";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".ico"] = "image/vnd.microsoft.icon";
	_mimeTypes[".ics"] = "text/calendar";
	_mimeTypes[".jar"] = "application/java-archive";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".js"] = "text/javascript";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".jsonld"] = "application/ld+json";
	_mimeTypes[".mid"] = "audio/midi";
	_mimeTypes[".midi"] = "audio/x-midi";
	_mimeTypes[".mjs"] = "text/javascript";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mpeg"] = "video/mpeg";
	_mimeTypes[".mpkg"] = "application/vnd.apple.installer+xml";
	_mimeTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
	_mimeTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_mimeTypes[".odt"] = "application/vnd.oasis.opendocument.text";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".ogv"] = "video/ogg";
	_mimeTypes[".ogx"] = "application/ogg";
	_mimeTypes[".opus"] = "audio/opus";
	_mimeTypes[".otf"] = "font/otf";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".php"] = "application/x-httpd-php";
	_mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	_mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_mimeTypes[".rar"] = "application/vnd.rar";
	_mimeTypes[".rtf"] = "application/rtf";
	_mimeTypes[".sh"] = "application/x-sh";
	_mimeTypes[".svg"] = "image/svg+xml";
	_mimeTypes[".tar"] = "application/x-tar";
	_mimeTypes[".tif"] = "image/tiff";
	_mimeTypes[".tiff"] = "image/tiff";
	_mimeTypes[".ts"] = "video/mp2t";
	_mimeTypes[".ttf"] = "font/ttf";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".vsd"] = "application/vnd.visio";
	_mimeTypes[".wav"] = "audio/wav";
	_mimeTypes[".weba"] = "audio/webm";
	_mimeTypes[".webm"] = "video/webm";
	_mimeTypes[".webp"] = "image/webp";
	_mimeTypes[".woff"] = "font/woff";
	_mimeTypes[".woff2"] = "font/woff2";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".xls"] = "application/vnd.ms-excel";
	_mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	_mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".xul"] = "application/vnd.mozilla.xul+xml";
	_mimeTypes[".zip"] = "application/zip";
	_mimeTypes[".3gp"] = "video/3gpp";
	_mimeTypes[".3g2"] = "video/3gpp2";
	_mimeTypes[".7z"] = "application/x-7z-compressed";
}
