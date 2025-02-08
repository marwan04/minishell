/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:26 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/08 20:47:49 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdup(const char *src)
{
	int		len;
	int		i;
	char	*dest;

	len = 0;
	while (src[len] != '\0')
	{
		len++;
	}
	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
