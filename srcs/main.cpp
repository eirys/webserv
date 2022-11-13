/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 15:30:51 by etran             #+#    #+#             */
/*   Updated: 2022/10/25 00:36:52 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
# include "debug.hpp"

int	__is_child;

int main(int argc, char **argv, char* const* env)
{
	std::string configFile("tester/testFiles/valid/default.conf");

	if (argc != 2) {
		std::cerr << "usage : ./webserv [a configuration file]" << std::endl;
		if (argc > 2)
			return (EXIT_FAILURE);
		std::cout << "Using default configuration file" << std::endl;
	} else {
		configFile = argv[1];
	}
	__is_child = 0;
	try {
		Webserv	webserv(configFile, env);
	} catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		std::cerr << "Errno value: " << errno << NL;
		return (EXIT_FAILURE);
	}
	return (__is_child);
}
