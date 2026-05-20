/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 16:14:39 by aleriaza          #+#    #+#             */
/*   Updated: 2025/12/25 16:56:14 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 500
#include "minishell.h"

/* Global variable for signal handling */
int	g_signal_received = 0;

/* Handle SIGINT (Ctrl-C) in interactive mode */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Setup signal handlers for interactive mode */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_tmp_signals(void)
{
	struct sigaction	sa_ignore;
	struct sigaction	sa_old;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_old);
}
