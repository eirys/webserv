/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 16:35:16 by etran             #+#    #+#             */
/*   Updated: 2022/11/09 19:34:08 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "Response.hpp"

/* == PUBLIC =============================================================== */

CgiHandler::CgiHandler(Response& response) :
	_program(response.getCgiProgram()),
	_in(-1),
	_out(-1),
	_env(*response.getEnv()),
	_script(response.getBuiltPath()),
	_requestbody(response.getRequest()->getBody()) {
		if (!pathIsAccessible(_script))
			throw (NOT_FOUND);
		setupSigpipe(INIT_SIGNAL);
	}

CgiHandler::~CgiHandler() {
	_restore();
	setupSigpipe(RESET_SIGNAL);
	DEBUG("Leaving CGI!!");
}

// Getter ---------------------------------------

/* Program that'll be executed */
const std::string& CgiHandler::getProgram() const {
	return (_program);
}

/* Name of the script */
const std::string& CgiHandler::getScript() const {
	return (_script);
}

/* Env with metavar */
char* const* CgiHandler::getEnv() const {
	return (_env);
}

// Cgi ------------------------------------------

bool CgiHandler::getCgiOutput(std::string& output) {
	int	pid;

	DEBUG("In CGI!!");
	//_env.display();
	_setupPipe();
	pid = fork();
	if (pid < 0) {
		throw RuntimeError("cgihandler getCgiOutput (fork) error");
	} else if (pid == 0) {
		/* Child */
		DEBUG("Child forked");
		usleep(900);
		_execute();
		__is_child = 1;
		return (true);
	} else {
		/* Parent */
		if (_waitForChild(pid)) {
			output = readFd(_fd_out[0]);
			close(_fd_out[0]);
			return (true);
		} else {
			close(_fd_out[0]);
			return (false);
		}
	}
}

/* == PRIVATE ============================================================== */

// Utils ----------------------------------------

void CgiHandler::_execute() {
	char* av[] = {
		const_cast<char*>(getProgram().c_str()),
		const_cast<char*>(getScript().c_str()),
		0 };
	_redirectToPipe();
	execve(av[0], av, getEnv());
	//DEBUG ("FAILED EXECVE!!!");
	if (close(_fd_in[0]))
		throw RuntimeError("cgihandler execute (close) error");
	if (close(_fd_out[1]))
		throw RuntimeError("cgihandler execute (close) error");
	//throw RuntimeError("execve failed in cgi");
}

/* Restoring input output in case duping happened */
void CgiHandler::_restore() {
	if (_in == -1 && _out == -1)
		return ;

	/* Restoring input */
	if (dup2(_in, STDIN_FILENO) < 0)
		throw RuntimeError("cgihandler _restore (dup2) error");
	if (close(_in))
		throw RuntimeError("cgihandler _restore (close) error");
	/* Restoring output */
	if (dup2(_out, STDOUT_FILENO) < 0)
		throw RuntimeError("cgihandler _restore (dup2) error");
	if (close(_out))
		throw RuntimeError("cgihandler _restore (close) error");
}

/* Redirect output to the pipe -> building response */
void CgiHandler::_redirectToPipe() {
	if (close(_fd_in[1]) < 0)
		throw RuntimeError("cgihandler _redirectToPipe (close) error");
	if ((_in = dup(STDIN_FILENO)) < 0) /* Save old stdout */
		throw RuntimeError("cgihandler _redirectToSock (dup) error");
	if (dup2(_fd_in[0], STDIN_FILENO) < 0) /* Output will also be on fd_out[0] */
		throw RuntimeError("cgihandler _redirectToPipe (dup2) error");

	if (close(_fd_out[0]) < 0) /* Don't need input side */
		throw RuntimeError("cgihandler _redirectToPipe (close) error");
	if ((_out = dup(STDOUT_FILENO)) < 0) /* Save old stdout */
		throw RuntimeError("cgihandler _redirectToSock (dup) error");
	if (dup2(_fd_out[1], STDOUT_FILENO) < 0) /* Output will also be on fd_out[0] */
		throw RuntimeError("cgihandler _redirectToPipe (dup2) error");
}

/* Wait for child execution */
bool CgiHandler::_waitForChild(int pid) {
	/* Parent */
	int		wstatus;
	time_t	start = std::time(0);

	_writeToStdin();
	while (1) {
		if (waitpid(pid, &wstatus, WNOHANG) == pid)
			break ; /* OK */
		usleep(50);
		if (std::difftime(time(0), start) >= TIMEOUT_LIMIT) {
			kill(pid, SIGINT);
			break ;
		}
	}
	if (WIFEXITED(wstatus)) { /* check if exit was ok */
		if (WEXITSTATUS(wstatus) != EXIT_FAILURE) {
			DEBUG ("Exited waitforchild smoothh..... ==== ");
			return (true);
		} else {
			DEBUG ("Child execve failed");
		}
	}
	return (false);
}

/* Open pipe and set to non blocking */
void CgiHandler::_setupPipe() {
	if (pipe(_fd_in) < 0)
		throw RuntimeError("cgihandler getCgiOutput (pipe) error");
	if (pipe(_fd_out) < 0)
		throw RuntimeError("cgihandler getCgiOutput (pipe) error");
}

/* Close unused pipe ends */
void CgiHandler::_setupParentIo() {
	if (close(_fd_in[0]) < 0)
		throw INTERNAL_SERVER_ERROR;
	if (close(_fd_out[1]) < 0) /* Don't need output side */
		throw INTERNAL_SERVER_ERROR;
}

/* Write request body content to child stdin */
void CgiHandler::_writeToStdin() {
	_setupParentIo();
	if (write(_fd_in[1], _requestbody.c_str(), _requestbody.size()) < 0)
		throw RuntimeError("cgihandler _writeToStdin (write) error");
	if (close(_fd_in[1]) < 0)
		throw RuntimeError("cgihandler _writeToStdin (close) error");
}
