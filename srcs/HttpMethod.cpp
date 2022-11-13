#include "HttpMethod.hpp"

HttpMethod::HttpMethod()
{
	_methods[GET] = "GET";
	_methods[POST] = "POST";
	_methods[DELETE] = "DELETE";
}

HttpMethod::~HttpMethod() {}

bool        HttpMethod::isHttpMethod(std::string& str)
{
    std::map<t_method, std::string>::const_iterator ite;

    for (ite = _methods.begin(); ite != _methods.end(); ite++)
    {
        if (ite->second == str)
            return (true);
    }
	return (false);
}

bool        HttpMethod::isHttpMethod(std::string const& str)
{
    std::map<t_method, std::string>::const_iterator ite;

    for (ite = _methods.begin(); ite != _methods.end(); ite++)
    {
        if (ite->second == str)
            return (true);
    }
	return (false);
}

t_method	HttpMethod::getMethod(const std::string& str) const
{
    std::map<t_method, std::string>::const_iterator ite;

    for (ite = _methods.begin(); ite != _methods.end(); ite++)
    {
        if (ite->second == str)
            return (ite->first);
    }
	return (NO_METHOD);
}

std::string const&		HttpMethod::getMethod(t_method method) const
{
    std::map<t_method, std::string>::const_iterator ite;

    ite = _methods.find(method);
    // if (ite != _methods.end())
        return (ite->second);
    // return ("");
}

std::string&	HttpMethod::operator[](t_method method)
{
	std::map<t_method, std::string>::iterator	ite;

	ite = _methods.find(method);
	return (ite->second);
}
