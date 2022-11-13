/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:45:14 by etran             #+#    #+#             */
/*   Updated: 2022/11/09 14:59:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

bool _triggered;

void	_sigHandler(int sig) {
	(void) sig;
	_triggered = true;
}

void	_ignoreSigpipe(int sig) {
	(void) sig;
}

void	setupSignal(int state) {
	void	(*oldHandler_SigInt)(int) = 0;
	void	(*oldHandler_SigTerm)(int) = 0;

	if (state == INIT_SIGNAL) {
		_triggered = false;
		oldHandler_SigInt = signal(SIGINT, _sigHandler);
		oldHandler_SigTerm = signal(SIGTERM, _sigHandler);
	} else if (state == RESET_SIGNAL) {
		signal(SIGINT, oldHandler_SigInt);
		signal(SIGTERM, oldHandler_SigTerm);
	}
}

bool	getTriggeredValue() {
	return (_triggered);
}

void	setupSigpipe(int state) {
	void	(*oldHandler_SigPipe)(int) = 0;

	if (state == INIT_SIGNAL) {
		oldHandler_SigPipe = signal(SIGPIPE, _ignoreSigpipe);
	} else if (state == RESET_SIGNAL) {
		signal(SIGPIPE, oldHandler_SigPipe);
	}
}
