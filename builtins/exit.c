/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 01:26:09 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/05/04 23:30:42 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_overflow(unsigned long long value, int sign, char digit)
{
	if (value > 922337203685477580ULL)
		return (1);
	if (value == 922337203685477580ULL)
	{
		if ((sign == 1 && (digit - '0') > 7)
			|| (sign == -1 && (digit - '0') > 8))
			return (1);
	}
	return (0);
}

int	is_valid_exit_arg(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	value;

	if (!str)
		return (0);
	i = 0;
	sign = 1;
	value = 0;
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
		if (!ft_isdigit(str[i]) || is_overflow(value, sign, str[i]))
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

int	handle_exit(char **args, int *last_exit)
{
	if (!args[1])
		return (*last_exit);
	if (!is_valid_exit_arg(args[1]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (2);
	}
	if (args[2])
	{
		write(1, "exit\n", 5);
		ft_putendl_fd("bash: exit: too many arguments\n", 2);
		*last_exit = 1;
		return (1);
	}
	*last_exit = (unsigned char)ft_atoi(args[1]);
	return (*last_exit);
}
