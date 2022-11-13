#ifndef COOKIE_HPP
# define COOKIE_HPP

# include "utils.hpp"

/******************************************************************************/
/*                                CLASS COOKIE                                */
/******************************************************************************/

class Cookie
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string		_name;
		std::string		_value;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Cookie();
		Cookie(const std::string& name, const std::string& value);
		Cookie(const Cookie& other);
		Cookie&			operator=(const Cookie& other);
		~Cookie();

						/*-------    Setter   -------*/
		void			setCookie(const std::string& name, const std::string& value);

						/*-------    Getter   -------*/
		std::string		getName() const;
		std::string		getValue() const;

						/*-------     Utils   -------*/
		void			display() const;
};

#endif
