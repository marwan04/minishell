/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:08:28 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/21 23:50:57 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

void	handle_sigint(int sig)
{
	(void) sig;
	g_sig_int = 1;
	write(1, "\n", 1);
}

void	init_signals(t_minishell *data)
{
	if(g_sig_int)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig_int = 0;
		data->last_exit_status = 1;
	}
}

void 	signals_handler(void)
{
    struct sigaction sa;

    rl_catch_signals = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags   = 0;
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
