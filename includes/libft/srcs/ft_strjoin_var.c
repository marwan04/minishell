/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:40:15 by malrifai          #+#    #+#             */
/*   Updated: 2025/03/14 15:29:56 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin_var(int count, ...)
{
	va_list	args;
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	if (count == 0)
		return (NULL);
	va_start(args, count);
	result = va_arg(args, char *);
	if (!result)
		result = "";
	while (i < count - 1)
	{
		temp = ft_strjoin(result, va_arg(args, char *));
		if (i > 0)
			free(result);
		result = temp;
		i++;
	}
	va_end(args);
	return (result);
}
