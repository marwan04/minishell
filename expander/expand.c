/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/10 16:40:01 by malrifai         ###   ########.fr       */
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

