/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:48:11 by malrifai          #+#    #+#             */
/*   Updated: 2024/11/10 00:29:10 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	putunbr_fd(unsigned int n)
{
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, 1);
		ft_putchar_fd(n % 10 + '0', 1);
	}
	else
		ft_putchar_fd(n + '0', 1);
}

int	print_unsigned(unsigned int n)
{
	unsigned int	nb;
	unsigned int	i;

	nb = n;
	i = 1;
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	putunbr_fd(n);
	return (i);
}
