# include "Purchase.hpp"

Purchase::Purchase() {}

Purchase::Purchase(const Purchase& other)
{
	*this = other;
}

Purchase&	Purchase::operator=(const Purchase& other)
{
	if (this != &other)
	{
		_name = other.getName();
		_hamster = other.getHamster();
		_color = other.getColor();
		_id = other.getId();
	}
	return (*this);
}

Purchase::~Purchase() {}

void	Purchase::display() const
{
    std::cout << "name = " << _name << std::endl;    
    std::cout << "hamster = " << _hamster << std::endl;    
    std::cout << "color = " << _color << std::endl;    
}

bool	Purchase::isComplete() const
{
    return (!_name.empty() && !_hamster.empty() && !_color.empty() && !_id.empty());
}

std::string		Purchase::getName() const
{
    return (_name);
}

std::string		Purchase::getHamster() const
{
    return (_hamster);
}

std::string		Purchase::getColor() const
{
    return (_color);
}

std::string		Purchase::getId() const
{
	return (_id);
}

void	Purchase::setName(const std::string& name)
{
	_name = name;
}

void	Purchase::setHamster(const std::string& hamster)
{
	_hamster = hamster;
}

void	Purchase::setColor(const std::string& color)
{
	_color = color;
}

void	Purchase::setId(const std::string& id)
{
	_id = id;
}
