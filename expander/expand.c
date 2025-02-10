/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/10 18:22:09 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
char *expand_tilde(char *token)
{
    char *home;
    char *expanded_token;
    
    if (!token || token[0] != '~')
        return ft_strdup(token);
    home = getenv("HOME");
    if (!home)
        return ft_strdup(token);
    if (token[1] == '\0')
        return ft_strdup(home);
    expanded_token = ft_strjoin(home, token + 1);
    return expanded_token;
}

char *expand_variables(char *token, int last_exit_status)
{
    char *expanded;
    char *exit_status;
    char *var_name;
    char *var_value;
    char c[2];
    int i;
    int start;
    
    if (!token || !ft_strchr(token, '$'))
        return ft_strdup(token);
    expanded = ft_strdup("");
    i = 0;
    while (token[i])
    {
        if (token[i] == '\'')
        {
            start = i++;
            while (token[i] && token[i] != '\'') 
                i++;
            if (token[i] == '\'')
                i++;
            expanded = ft_strjoin_free(expanded, ft_substr(token, start, i - start));
        }
        else if (token[i] == '$')
        {
            start = ++i;
            if (token[start] == '?')
            {
                exit_status = ft_itoa(last_exit_status);
                expanded = ft_strjoin_free(expanded, exit_status);
                free(exit_status);
                i++;
            }
            else
            {
                while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
                    i++;
                var_name = ft_substr(token, start, i - start);
                var_value = getenv(var_name);
                if (var_value)
                     expanded = ft_strjoin_free(expanded, var_value);
                else
                    expanded = ft_strjoin_free(expanded, "");
                free(var_name);
            }
        }
        else
        {
            c[0] = token[i];
            c[1] = '\0';
            expanded = ft_strjoin_free(expanded, c);
            i++;
        }
    }
    return expanded;
}

void expand_tokens(t_token *tokens, int last_exit_status)
{
    while (tokens)
    {
        // Apply expansions
        char *expanded = expand_tilde(tokens->value);
        expanded = expand_variables(expanded, last_exit_status);

        if (expanded != tokens->value)
        {
            free(tokens->value);
            tokens->value = expanded;
        }
        tokens = tokens->next;
    }
}
