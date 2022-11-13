/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:24:02 by etran             #+#    #+#             */
/*   Updated: 2022/10/31 13:48:37 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Env.hpp"

/* == PUBLIC =============================================================== */

Env::Env() :
	_env(0) {}

Env::Env(char* const* env) :
	_env(0),
	_envset(_setEnv(env)) {
		_convertEnv();
	}

Env::Env(const Env& old) :
	_env(0),
	_envset(old._envset) {
		_convertEnv();
	}

Env::~Env() {
	if (_env)
		_clearEnv();
}

// Getter ---------------------------------------

/* Returns _env */
Env::operator char* const*() const {
	return (const_cast<char* const*>(_env));
}

Env::operator const char* const*() const {
	return (const_cast<const char* const*>(_env));
}

/* Finding key in environment */
std::string Env::getEnv(const std::string& key) const {
	for (envSet::const_iterator it = _envset.begin(); it != _envset.end(); it++) {
		std::string		elem(*it);
		std::string		curr_key = elem.substr(0, elem.find('='));
		if (key == curr_key)
			return (elem);
	}
	return ("");
}

// Modifiers ------------------------------------

/* Adding to env unpaired key-value */
//void Env::addParam(const std::string& key, const std::string& val) {
//	std::string	pair = key + "=" + val;
//	_envset.insert(const_cast<char*>(pair.c_str()));
//	if (_env)
//		_updateEnv();
//}

void Env::addParam(const std::string key, const std::string val) {
	std::string	pair = key + "=" + val;
	_envset.insert(const_cast<char*>(pair.c_str()));
	if (_env)
		_updateEnv();
}

/* Adding to env already paired up key-value */
void Env::addParam(const std::string& keyval) {
	_envset.insert(const_cast<char*>(keyval.c_str()));
	if (_env)
		_updateEnv();
}

// Debug ----------------------------------------

void Env::display() const {
	size_t len = _getEnvSize(_env);
	std::cerr
			<< " ----------------------------- Env -----------------------" <<NL;
	for (size_t i = 0; i < len; i++)
		std::cerr
			<< "	env["<<i<<"] = "<<_env[i]<<NL;
	std::cout << " ----------------------------- End -----------------------" <<NL;
}

/* == PRIVATE ============================================================== */

// Utils ----------------------------------------

/* Create _envset from external env */
Env::envSet Env::_setEnv(char* const* env) const {
	size_t	env_size = _getEnvSize(env);
	envSet	set;

	for (size_t i = 0; i < env_size; i++)
		set.insert(env[i]);
	return (set);
}

/* Get env size from external env */
size_t Env::_getEnvSize(char* const* env) const {
	size_t	i = 0;
	while (env[i])
		i++;
	return (i);
}

/* Delete _env */
void Env::_clearEnv() {
	if (!_env)
		return ;
	size_t	len = _getEnvSize(_env);
	for (size_t i = 0; i < len; i++)
		delete[] _env[i];
	delete[] _env;
}

/* Convert set (_envset) to array (_env) */
void Env::_convertEnv() {
	char**	array = new char*[_envset.size() + 1];
	size_t	i = 0;

	for (envSet::const_iterator it = _envset.begin(); it != _envset.end(); it++) {
		array[i] = clone_str(*it);
		i++;
	}
	array[i] = 0;
	_env = array;
}

/* Update when adding */
void Env::_updateEnv() {
	if (!_env)
		return ;
	_clearEnv();
	_convertEnv();
}
