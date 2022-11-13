#include "Cookie.hpp"

Cookie::Cookie() {}

Cookie::Cookie(const std::string& name, const std::string& value)
{
	setCookie(name, value);
}

Cookie::Cookie(const Cookie& other)
{
	*this = other;
}

Cookie&		Cookie::operator=(const Cookie& other)
{
	if (this != &other)
	{
		_name = other.getName();
		_value = other.getValue();
	}
	return (*this);
}

Cookie::~Cookie() {}

void	Cookie::setCookie(const std::string& name, const std::string& value)
{
	size_t	totalSize;

	totalSize = name.size() + value.size() + 1;
	if (totalSize > 4096) /* Limit 4096 bytes */
		throw(BAD_REQUEST);
	_name = name;
	_value = value;
}

std::string		Cookie::getName() const
{
	return (_name);
}

std::string		Cookie::getValue() const
{
	return (_value);
}

void	Cookie::display() const
{
	std::cout << _name << " = " << _value << std::endl;
}
