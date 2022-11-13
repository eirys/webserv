#ifndef HTTP_METHOD_HPP
# define HTTP_METHOD_HPP

# include <string>
# include <map>

/******************************************************************************/
/*                                 ENUMS                                      */
/******************************************************************************/

typedef enum e_method
{
	GET			= 0,
	POST		= 1,
	DELETE		= 2,
	ALLOWED_METHODS_COUNT = 3,
	NO_METHOD	= 4,
} t_method;

/******************************************************************************/
/*                            CLASS HTTP METHOD                               */
/******************************************************************************/

class HttpMethod
{
    private:
	    std::map<t_method, std::string>		_methods;

    public:
        HttpMethod();
        ~HttpMethod();
        bool                    isHttpMethod(std::string& str);
        bool                    isHttpMethod(std::string const& str);
		t_method				getMethod(const std::string& str) const;
		std::string const&		getMethod(t_method method) const;
		std::string&			operator[](t_method method);
};

#endif