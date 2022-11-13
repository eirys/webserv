/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:06 by etran             #+#    #+#             */
/*   Updated: 2022/11/07 11:30:52 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

//# include <stdexcept>
# include <exception>
# include <unistd.h>
# include <string>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "Env.hpp"
# define TIMEOUT_LIMIT 5 /* Timeout in seconds */

class Response;

class CgiHandler {
	public:
		CgiHandler(Response& response);
		virtual ~CgiHandler();

		/* -- Getter -------------------------------------------------------- */
		const std::string&	getProgram() const;
		const std::string&	getScript() const;
		char* const*		getEnv() const;

		bool				getCgiOutput(std::string& output);

		class RuntimeError : public virtual std::exception {
			public:
				RuntimeError(const char* msg) : _msg(msg) {}

				const char*  what() const throw() {
					return (_msg);
				}
			private:
				const char*	_msg;
		};

	private:
		/* -- Utils --------------------------------------------------------- */
		void				_execute();
		void				_restore();
		void				_redirectToPipe();
		void				_setupPipe();
		void 				_writeToStdin();
		void				_setupParentIo();
		bool				_waitForChild(int pid);

		/* -- Data members -------------------------------------------------- */
		std::string			_program;
		int					_in;
		int					_out;
		const Env&			_env;
		std::string			_script;
		int					_fd_in[2];
		int					_fd_out[2];
		std::string			_requestbody;
};

#endif
