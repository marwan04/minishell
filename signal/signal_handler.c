/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:08:28 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 18:43:48 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

void	handle_sigint(int signo)
{
	(void)signo;
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

	g_sig_int = 0;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) < 0)
		perror("sigaction(SIGINT)");
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
		perror("sigaction(SIGQUIT)");
}

void	ign_sig(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	def_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
