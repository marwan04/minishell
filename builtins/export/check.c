/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:00:41 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/22 20:11:12 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_valid_identifier(char *key)
{
	int i = 0;

	if (!key || !ft_isalpha(key[0]) || key[0] == '=')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int has_space_before_equal(char *arg)
{
	int i = 0;

	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int has_space_after_equal(char *equal)
{
	int i;
	int flag;
	char quote;

	i = 0;
	flag = 0;
	while (equal[i] != '=')
		i++;
	i++;
	while (equal[i] == ' ')
	{
		flag = 2;
		i++;
	}
	if ((equal[i] == '"' || equal[i] == '\'') && flag == 2)
	{
		quote = equal[i];
		i++;
		while (equal[i] && equal[i] != quote)
		{
			if (equal[i] == ' ')
				flag = 1;
			i++;
		}
	}
	return (flag);
}
