/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 01:26:09 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/04/24 01:37:10 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_exit_arg(const char *str)
{
	int			i;
	int			sign;
	unsigned long long	value;

    i = 0;
    sign = 1;
    value = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (value > 922337203685477580ULL ||
			(value == 922337203685477580ULL &&
			((sign == 1 && (str[i] - '0') > 7) || (sign == -1 && (str[i] - '0') > 8))))
			return (0);
		value = value * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

void	ft_exit(t_minishell *data)
{
	if (!data->ast_root->args[1])
		ft_free(data, data->last_exit_status, "exit\n");
    if (!is_valid_exit_arg(data->ast_root->args[1]))
    {
        ft_putstr_fd("bash: exit: ", 2);
        ft_putstr_fd(data->ast_root->args[1], 2);
        ft_putendl_fd(": numeric argument required", 2);
        ft_free(data, 2, "exit\n");
    }
	if (data->ast_root->args[2])
	{
		write(1, "exit\n", 5);
		ft_putendl_fd("bash: exit: too many arguments\n", 2);
		data->last_exit_status = 1;
		return ;
	}
	data->last_exit_status = (unsigned char)ft_atoi(data->ast_root->args[1]);
    ft_free(data, data->last_exit_status, "exit\n");
}

