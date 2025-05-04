/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:08:28 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 02:10:44 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

#include <readline/readline.h>
#include <unistd.h>

void handle_sigint(int signo)
{
    (void)signo;
    g_sig_int = 1;
	close(STDIN_FILENO);
}

int check_signal()
{
    if (g_sig_int)
    {
        dup2(STDOUT_FILENO, STDIN_FILENO);
		g_sig_int = 0;
        return (1);
    }
    return (0);
}

void init_signals(void)
{
    struct sigaction sa = {0};

    rl_catch_signals = 0;   // we’ll handle SIGINT ourselves
    g_sig_int = 0;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;        // *no* SA_RESTART, so readline gets EINTR

    if (sigaction(SIGINT, &sa, NULL) < 0)
        perror("sigaction(SIGINT)");

    sa.sa_handler = SIG_IGN;   // ignore SIGQUIT (Ctrl-\)
    if (sigaction(SIGQUIT, &sa, NULL) < 0)
        perror("sigaction(SIGQUIT)");
}
