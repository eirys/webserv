#include "Token.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Token::Token():
	_type(FILE_START),
	_value(""),
	_line(0) {}

Token::Token(tokenType type, tokenValue value, unsigned int line):
	_type(type),
	_value(value),
	_line(line)	{}

Token::Token(Token const &other)
{
	*this = other;
}

Token::~Token() {}

Token		&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_type = other.getType();
		_value = other.getValue();
		_line = other.getLineNbr();
	}
	return (*this);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Token::tokenType	Token::getType() const
{
	return (this->_type);
}

Token::tokenValue	Token::getValue() const
{
	return (this->_value);
}

size_t	Token::getLineNbr() const
{
	return (this->_line);
}

std::string		Token::getLineStr() const
{
	return (convertSizeToString(_line));
}

std::string		Token::getLineStr(size_t nb) const
{
	return (convertSizeToString(_line + nb));
}
