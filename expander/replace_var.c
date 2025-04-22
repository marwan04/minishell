/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:36:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 17:36:17 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_split_and_null(char **splited)
{
	ft_free_double_list(splited);
	return (NULL);
}

char	*init_expanded_string(void)
{
	char	*expanded;

	expanded = ft_strdup("");
	return (expanded);
}

char	*join_splitted_parts(char *expanded, char **splited)
{
	char	*temp;
	int		j;

	j = 0;
	while (splited[j])
	{
		temp = ft_strjoin(expanded, splited[j]);
		free(expanded);
		if (!temp)
			return (free_split_and_null(splited));
		expanded = temp;
		if (splited[j + 1])
		{
			temp = ft_strjoin(expanded, " ");
			free(expanded);
			if (!temp)
				return (free_split_and_null(splited));
			expanded = temp;
		}
		free(splited[j]);
		j++;
	}
	free(splited);
	return (expanded);
}

char	*expand_replace_var(char *var_name,
			int preserve_spaces, t_env *env)
{
	char		*var_value;
	char		**splited;
	char		*expanded;

	var_value = get_env_value(env, var_name);
	if (!var_value)
		return (ft_strdup(""));
	if (preserve_spaces)
		return (ft_strdup(var_value));
	splited = ft_split(var_value, ' ');
	if (!splited)
		return (NULL);
	expanded = init_expanded_string();
	if (!expanded)
		return (free_split_and_null(splited));
	expanded = join_splitted_parts(expanded, splited);
	return (expanded);
}
