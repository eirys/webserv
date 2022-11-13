/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:25:28 by etran             #+#    #+#             */
/*   Updated: 2022/10/17 18:14:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_HPP
# define ENV_HPP

# include <set>
# include "utils.hpp"
# include "debug.hpp"

class Env {
	public:
		/* -- Typedef ------------------------------------------------------- */
		typedef			std::set<std::string>		envSet;

		Env();
		explicit Env(char* const* env);
		Env(const Env& old);
		~Env();

		/* -- Getter -------------------------------------------------------- */
		operator 		char* const*() const;
		operator 		const char* const* () const;
		std::string		getEnv(const std::string& key) const;
		//char* const*	getEnv() const;
		//operator char* const*();

		/* -- Modifiers ----------------------------------------------------- */
	//	void			addParam(const std::string& key, const std::string& val);
		void			addParam(const std::string key, const std::string val);
		void			addParam(const std::string& keyval);

		/* -- Debug --------------------------------------------------------- */
		void			display() const;

	private:
		/* -- Utils --------------------------------------------------------- */
		envSet			_setEnv(char* const* env) const;
		size_t			_getEnvSize(char* const* env) const;
		void			_clearEnv();
		void			_convertEnv();
		void			_updateEnv();

		/* -- Data members -------------------------------------------------- */
		char**			_env;
		envSet			_envset;
};

#endif
