#ifndef PURCHASE_HPP
# define PURCHASE_HPP

# include "utils.hpp"
# include <vector>

# define PURCHASE_ID_LENGTH 8

/******************************************************************************/
/*                               CLASS PURCHASE                               */
/******************************************************************************/

class Purchase
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string		_name;
		std::string		_hamster;
		std::string		_color;
		std::string		_id;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Purchase();
		Purchase(const Purchase& other);
		Purchase&		operator=(const Purchase& other);
		~Purchase();

						/*-------    Setter   -------*/
		void			setName(const std::string& name);
		void			setHamster(const std::string& hamster);
		void			setColor(const std::string& color);
		void			setId(const std::string& id);

						/*-------    Getter   -------*/
		std::string		getName() const;
		std::string		getHamster() const;
		std::string		getColor() const;
		std::string		getId() const;

						/*-------     Utils   -------*/
		void			display() const;
		bool			isComplete() const;
};

#endif
