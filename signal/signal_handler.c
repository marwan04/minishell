/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:08:28 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 15:27:30 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

void	handle_sigint(int sig)
{
	(void) sig;
	g_sig_int = 1;
	close(STDIN_FILENO);
}

int	check_signal(t_minishell *data)
{
	if (g_sig_int)
	{
		dup2(STDOUT_FILENO, STDIN_FILENO);
		g_sig_int = 0;
		data->last_exit_status = 130;
		return (1);
	}
	return (0);
}

void	init_signals(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
