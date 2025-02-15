/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:51:48 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/15 20:57:31 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* This function for the following
1- Keeps track of whether we are inside single (') or double (") quotes.
2- Ensures that double quotes (") preserve spaces in expanded variables.
3- Prevents expansion inside single quotes ('), (same as Bash).
*/
void	expand_track_quotes(t_expand *expand, char c)
{
	if ((c == '"' || c == '\'') && expand->quote == 0)
	{
		expand->quote = c;
		if (expand->quote == '"')
			expand->preserve_spaces = 1;
	}
	else if (c == expand->quote)
	{
		expand->quote = 0;
		expand->preserve_spaces = 0;
	}
}
/* This function for the following
1- Identifies and extracts the variable name after $.
2- Handles special case $?, which represents the exit status of the last command.
*/
char	*expand_extract_var(t_expand *expand, char *token, int last_exit_status)
{
	expand->start = ++expand->i;
	if (token[expand->start] == '?')
	{
		expand->exit_status = ft_itoa(last_exit_status);
		expand->i++;
		return (expand->exit_status);
	}
	while (token[expand->i] && (ft_isalnum(token[expand->i]) \
		|| token[expand->i] == '_'))
		expand->i++;
	return (ft_substr(token, expand->start, expand->i - expand->start));
}

/* This function for the following
1- Retrieves the variable value using getenv().
2- Handles spaces inside variables:
3- Preserve spaces if the variable is inside " ".
4- Split spaces into separate arguments if the variable is unquoted.
*/
char	*expand_replace_var(char *var_name, int preserve_spaces)
{
	char	*var_value;
	char	*expanded;
	char	**splited;
	int		j;

	var_value = getenv(var_name);
	if (!var_value)
		return (ft_strdup(""));
	if (preserve_spaces)
		return (ft_strdup(var_value));
	expanded = ft_strdup("");
	splited = ft_split(var_value, ' ');
	j = 0;
	while (splited[j])
	{
		expanded = ft_strjoin_free(expanded, splited[j]);
		if (splited[j + 1])
			expanded = ft_strjoin_free(expanded, " ");
		free(splited[j]);
		j++;
	}
	free(splited);
	return (expanded);
}

/* This function for the following
1- Handles characters that are not part of a variable ($VAR).
2- Ensures that the expanded string is built correctly.
*/
void	expand_append_char(t_expand *expand, char c)
{
	expand->c[0] = c;
	expand->c[1] = '\0';
	expand->expanded = ft_strjoin_free(expand->expanded, expand->c);
}

/* This function for the following
1- Loops through token character by character.
2- Tracks quotes (' ") and applies variable expansion accordingly.
3- Calls helper functions for:
 - Handling quotes (expand_track_quotes())
 - Extracting variables (expand_extract_var())
 - Replacing variables (expand_replace_var())
 - Appending normal characters (expand_append_char())
*/
char	*expand_variables(char *token, int last_exit_status)
{
	t_expand	exp;

	if (!token || !ft_strchr(token, '$'))
		return (ft_strdup(token));
	exp.quote = 0;
	exp.preserve_spaces = 0;
	exp.expanded = ft_strdup("");
	exp.i = 0;
	while (token[exp.i])
	{
		expand_track_quotes(&exp, token[exp.i]);
		if (token[exp.i] == '$' && exp.quote != '\'')
		{
			exp.var_name = expand_extract_var(&exp, \
				token, last_exit_status);
			exp.var_value = expand_replace_var(exp.var_name, \
				exp.preserve_spaces);
			exp.expanded = ft_strjoin_free(exp.expanded, exp.var_value);
			free(exp.var_name);
			free(exp.var_value);
		}
		else
			expand_append_char(&exp, token[exp.i++]);
	}
	return (exp.expanded);
}

// char *expand_variables(char *token, int last_exit_status)
// {
//     t_expand expand;

//     if (!token || !ft_strchr(token, '$'))
//         return (ft_strdup(token));
//     expand.preserve_spaces = 0;
//     expand.quote = 0;
//     expand.expanded = ft_strdup("");
//     expand.i = 0;
//     while (token[expand.i])
//     {
//         if ((token[expand.i] == '"' || token[expand.i] == '\'') 
//				&& expand.quote == 0)
//         {
//             expand.quote = token[expand.i];
//             if (expand.quote == '"')
//                 expand.preserve_spaces = 1;
//         }
//         else if (token[expand.i] == expand.quote)
//         {
//             expand.quote = 0;
//             expand.preserve_spaces = 0;
//         }
//         if (token[expand.i] == '$' && expand.quote != '\'')
//         {
//             expand.start = ++expand.i;
//             if (!ft_isalpha(token[expand.start]) && 
//					token[expand.start] != '_' && token[expand.start] != '?')
//             {
//                 expand.expanded = ft_strjoin_free(expand.expanded, "$");
//                 continue;
//             }
//             if (token[expand.start] == '?')
//             {
//                 expand.exit_status = ft_itoa(last_exit_status);
//                 expand.expanded = ft_strjoin_free(expand.expanded, 
//						expand.exit_status);
//                 free(expand.exit_status);
//                 expand.i++;
//             }
//             else
//             {
//                 while (token[expand.i] && (ft_isalnum(token[expand.i])
//                    || token[expand.i] == '_'))
//                     expand.i++;
//                 expand.var_name = ft_substr(token, expand.start, 
//                     expand.i - expand.start);
//                 expand.var_value = getenv(expand.var_name);
//                 if (expand.var_value)
//                 {
//                     if (expand.preserve_spaces)
//                         expand.expanded = ft_strjoin_free(expand.expanded,
//                         expand.var_value);
//                     else
//                     {
//                         char **split = ft_split(expand.var_value, ' ');
//                         for (int j = 0; split[j]; j++)
//                         {
//                             expand.expanded = ft_strjoin_free
//                             (expand.expanded,split[j]);
//                             if (split[j + 1])
//                                 expand.expanded = ft_strjoin_free
//                                 (expand.expanded, " ");
//                             free(split[j]);
//                         }
//                         free(split);
//                     }
//                 }
//                 else
//                     expand.expanded = ft_strjoin_free(expand.expanded, "");
//                 free(expand.var_name);
//             }
//         }
//         else
//         {
//             expand.c[0] = token[expand.i];
//             expand.c[1] = '\0';
//             expand.expanded = ft_strjoin_free(expand.expanded, expand.c);
//             expand.i++;
//         }
//     }
//     return (expand.expanded);
// }
