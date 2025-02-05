/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:45:30 by malrifai          #+#    #+#             */
/*   Updated: 2024/11/10 00:20:55 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	print_hex(unsigned long value, int uppercase)
{
	char	*base;
	int		i;

	i = 0;
	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (value >= 16)
	{
		i += print_hex(value / 16, uppercase);
		ft_putchar_fd(base[value % 16], 1);
		i++;
	}
	else
	{
		ft_putchar_fd(base[value], 1);
		i++;
	}
	return (i);
}
